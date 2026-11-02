// Recover only the next small batch from an explicitly selected original
// initializer table. Run read-only; exports and game-derived bytes stay private.
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.AddressSet;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.HexFormat;
import java.security.MessageDigest;

public class ExportInitializerBatch extends GhidraScript {
    @Override public void run() throws Exception {
        var args=getScriptArgs();
        if(args.length!=5)throw new IllegalArgumentException("New private output, prior generation, dispatcher, table start/end required");
        var project=Path.of(getSourceFile().getAbsolutePath()).toRealPath().getParent().getParent().getParent();
        var research=project.resolve("research").toRealPath();
        var output=Path.of(args[0]).toAbsolutePath().normalize();
        var priorPath=Path.of(args[1]).toRealPath();
        if(!output.getParent().toRealPath().startsWith(research) || !priorPath.startsWith(research) || Files.exists(output) ||
           !output.toString().endsWith(".json") || Files.size(priorPath)>1024*1024)throw new IllegalArgumentException("Use bounded private research paths and preserve outputs");
        var prior=JsonParser.parseString(Files.readString(priorPath)).getAsJsonObject();
        if(!prior.get("sourceSha256").getAsString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalArgumentException("Executable identity differs");
        var known=new HashSet<Long>();
        for(var item:prior.getAsJsonArray("functions"))known.add(item.getAsJsonObject().get("entry").getAsLong());
        int functionLimit=prior.has("functionLimit")?prior.get("functionLimit").getAsInt():64;
        if(functionLimit!=64 && functionLimit!=128 && functionLimit!=256 && functionLimit!=512)throw new IllegalArgumentException("Unsupported explicit function limit");
        int rootLimit=Math.min(6,functionLimit-known.size());
        if(rootLimit<1)throw new IllegalArgumentException("No remaining function budget");
        var dispatcher=toAddr(args[2]);var start=toAddr(args[3]);var end=toAddr(args[4]);
        long bytes=end.subtract(start);
        if(!known.contains(dispatcher.getOffset()) || bytes<4 || bytes>1024 || (start.getOffset()&3)!=0 || (bytes&3)!=0 ||
           !start.getAddressSpace().equals(end.getAddressSpace()))throw new IllegalArgumentException("Use a bounded aligned table and admitted dispatcher");
        var function=getFunctionAt(dispatcher);
        if(function==null)throw new IllegalArgumentException("Dispatcher must be established");
        boolean hasStart=false,hasEnd=false;
        var instructions=currentProgram.getListing().getInstructions(function.getBody(),true);
        while(instructions.hasNext())for(var ref:instructions.next().getReferencesFrom()) {
            hasStart|=ref.getToAddress().equals(start);hasEnd|=ref.getToAddress().equals(end);
        }
        if(!hasStart || !hasEnd)throw new IllegalArgumentException("Dispatcher does not reference both table bounds");
        var roots=new ArrayList<String>();var targets=new JsonArray();var selected=new HashSet<Long>();
        int nonzero=0,remaining=0;
        for(var slot=start;slot.compareTo(end)<0;slot=slot.add(4)) {
            long value=Integer.toUnsignedLong(currentProgram.getMemory().getInt(slot));
            if(value==0)continue;
            nonzero++;
            if(known.contains(value))continue;
            remaining++;
            if(roots.size()==rootLimit || !selected.add(value))continue;
            var target=toAddr(Long.toHexString(value));
            var block=currentProgram.getMemory().getBlock(target);
            if(block==null || !block.isInitialized() || !block.isExecute())throw new IllegalArgumentException("Initializer target is not initialized executable memory");
            var existing=getFunctionAt(target);
            if(existing==null) {
                if(getFunctionContaining(target)!=null || currentProgram.getListing().getDefinedDataContaining(target)!=null ||
                   currentProgram.getListing().getInstructionContaining(target)!=null && currentProgram.getListing().getInstructionAt(target)==null)
                    throw new IllegalArgumentException("Initializer overlaps established code/data");
                var last=target.add(Math.min(65535,block.getEnd().subtract(target)));
                var command=new DisassembleCommand(target,new AddressSet(target,last),true);
                command.enableCodeAnalysis(false);
                if(!command.applyTo(currentProgram,monitor))throw new IllegalStateException(command.getStatusMsg());
                existing=createFunction(target,null);
                if(existing==null || !existing.getBody().getMinAddress().equals(target) || existing.getBody().getMaxAddress().compareTo(last)>0)
                    throw new IllegalArgumentException("Initializer "+target+" exceeds bounded window; recovered body "+(existing==null?"missing":existing.getBody().toString()));
            }
            roots.add(target.toString());
            var observation=new JsonObject();observation.addProperty("slot",slot.toString());observation.addProperty("target",target.toString());targets.add(observation);
        }
        if(roots.isEmpty())throw new IllegalArgumentException("No remaining initializer in this table");
        var exportArgs=new ArrayList<String>();exportArgs.add(output.toString());exportArgs.add(priorPath.toString());exportArgs.addAll(roots);
        runScript("ExportRecompilationClosure.java",exportArgs.toArray(new String[0]));
        var seed=JsonParser.parseString(Files.readString(output)).getAsJsonObject();
        var table=new JsonObject();table.addProperty("kind","bounded-static-initializer-table");table.addProperty("dispatcher",dispatcher.toString());
        table.addProperty("start",start.toString());table.addProperty("endExclusive",end.toString());table.addProperty("bytes",bytes);
        var rawTable=new byte[(int)bytes];
        if(currentProgram.getMemory().getBytes(start,rawTable)!=bytes)throw new IllegalStateException("Incomplete static table read");
        table.addProperty("analysisTableSha256",HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(rawTable)));
        var exported=new HashSet<String>();
        for(var item:seed.getAsJsonArray("functions"))exported.add(item.getAsJsonObject().get("entry").getAsString());
        var admittedTargets=new JsonArray();var deferredTargets=new JsonArray();
        for(var item:targets) {
            if(exported.contains(item.getAsJsonObject().get("target").getAsString()))admittedTargets.add(item);
            else deferredTargets.add(item);
        }
        table.addProperty("nonzeroEntries",nonzero);table.addProperty("remainingUnadmittedEntries",remaining);table.addProperty("runtimeObserved",false);
        table.add("selectedEntries",admittedTargets);table.add("deferredEntries",deferredTargets);
        seed.add("initializerTable",table);Files.writeString(output,seed.toString());
        println("Initializer batch exported locally: roots="+roots.size()+", remaining table entries="+remaining);
    }
}
