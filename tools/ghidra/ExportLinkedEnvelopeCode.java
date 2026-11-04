// Recover explicitly linked cold code inside one existing function envelope.
// Function membership stays unchanged; every recovered byte is rechecked by
// the original-byte and independent-decoder admission pipeline afterward.
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.scalar.Scalar;
import com.google.gson.JsonObject;
import com.google.gson.JsonArray;
import com.google.gson.JsonParser;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;

public class ExportLinkedEnvelopeCode extends GhidraScript {
    @Override public void run() throws Exception {
        var args=getScriptArgs();
        if(args.length<4 || args.length>5)throw new IllegalArgumentException("New output, prior seed, exact function, linked table and optional record count required");
        int recordCount=args.length==5?Integer.parseInt(args[4]):1;
        if(recordCount<1 || recordCount>8)throw new IllegalArgumentException("Select 1-8 explicitly inspected records");
        var project=Path.of(getSourceFile().getAbsolutePath()).toRealPath().getParent().getParent().getParent();
        var research=project.resolve("research").toRealPath();
        var input=Path.of(args[1]).toRealPath();var output=Path.of(args[0]).toAbsolutePath().normalize();
        if(!input.startsWith(research) || !output.getParent().toRealPath().startsWith(research) || Files.exists(output) ||
           Files.size(input)>8*1024*1024 || !output.toString().endsWith(".json"))throw new IllegalArgumentException("Use new bounded private research output");
        var prior=JsonParser.parseString(Files.readString(input)).getAsJsonObject();
        if(!prior.get("sourceSha256").getAsString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalArgumentException("Executable identity differs");
        var entry=toAddr(args[2]);var table=toAddr(args[3]);var function=getFunctionAt(entry);
        if(function==null || function.isExternal() || !entry.equals(function.getBody().getMinAddress()))throw new IllegalArgumentException("An established local entry is required");
        var first=function.getBody().getMinAddress();var last=function.getBody().getMaxAddress();
        if(last.subtract(first)>=65536)throw new IllegalArgumentException("Envelope exceeds admission budget");
        boolean linked=false;
        var prolog=currentProgram.getListing().getInstructions(new AddressSet(first,first.add(15)),true);
        while(prolog.hasNext()) {
            var instruction=prolog.next();if(!instruction.getMnemonicString().equals("PUSH"))continue;
            for(var value:instruction.getOpObjects(0))if(value instanceof Scalar scalar && scalar.getUnsignedValue()==table.getOffset())linked=true;
        }
        if(!linked || currentProgram.getMemory().getInt(table)!=-1)throw new IllegalArgumentException("The prolog must explicitly reference this bounded outermost record");
        var envelope=new AddressSet(first,last);var gaps=new AddressSet();int gapBytes=0;
        for(var item:prior.getAsJsonArray("functions"))if(toAddr(item.getAsJsonObject().get("entry").getAsString()).equals(entry)) {
            for(var region:item.getAsJsonObject().getAsJsonArray("layout")) {
                var value=region.getAsJsonObject();if(!value.get("kind").getAsString().equals("undefined"))continue;
                var low=toAddr(value.get("start").getAsString());int bytes=value.get("bytes").getAsInt();
                if(bytes<1 || bytes>256-gapBytes || !envelope.contains(low) || !envelope.contains(low.add(bytes-1)))throw new IllegalArgumentException("Bounded original envelope gaps required");
                for(int offset=0;offset<bytes;offset++)if(currentProgram.getListing().getDefinedDataContaining(low.add(offset))!=null || currentProgram.getListing().getInstructionContaining(low.add(offset))!=null)
                    throw new IllegalArgumentException("A retained undefined gap changed or contains established data/code");
                gaps.add(low,low.add(bytes-1));gapBytes+=bytes;
            }
        }
        if(gapBytes==0)throw new IllegalArgumentException("No retained undefined gap to recover");
        var records=new JsonArray();
        for(int record=0;record<recordCount;record++) {
            var base=table.add(record*12L);int outer=currentProgram.getMemory().getInt(base);
            if(record==0?outer!=-1:outer< -1 || outer>=record)throw new IllegalArgumentException("Scope records must refer only to an earlier enclosing record");
            var evidence=new JsonObject();evidence.addProperty("record",record);evidence.addProperty("enclosing",outer);var targets=new JsonArray();
            for(int field=0;field<2;field++) {
                long pointer=Integer.toUnsignedLong(currentProgram.getMemory().getInt(base.add(4L+field*4L)));
                var target=new JsonObject();target.addProperty("field",field);
                if(pointer==0) {target.addProperty("kind","null");targets.add(target);continue;}
                var address=first.getAddressSpace().getAddress(pointer);target.addProperty("address",address.toString());
                if(!envelope.contains(address)) {
                    var separate=getFunctionAt(address);var block=currentProgram.getMemory().getBlock(address);
                    if(separate==null || separate.isExternal() || block==null || !block.isExecute())throw new IllegalArgumentException("An outside target must already be a separate executable function");
                    target.addProperty("kind","separate-established-function");
                } else if(function.getBody().contains(address)) {
                    if(currentProgram.getListing().getInstructionAt(address)==null)throw new IllegalArgumentException("Body target is not established code");
                    target.addProperty("kind","established-body");
                } else {
                    var owner=getFunctionContaining(address);
                    if(!gaps.contains(address) || owner!=null && !owner.equals(function))throw new IllegalArgumentException("Decode only a retained envelope gap, never another function");
                    if(currentProgram.getListing().getInstructionAt(address)==null && !new DisassembleCommand(address,gaps,true).applyTo(currentProgram,monitor))throw new IllegalStateException("Bounded linked-code disassembly failed");
                    if(currentProgram.getListing().getInstructionAt(address)==null)throw new IllegalStateException("Linked target remains undecoded");
                    target.addProperty("kind","linked-gap-code");
                }
                targets.add(target);
            }
            evidence.add("targets",targets);records.add(evidence);
        }
        var exportArgs=new ArrayList<String>();exportArgs.add(output.toString());boolean included=false;
        for(var item:prior.getAsJsonArray("functions")) {String address=item.getAsJsonObject().get("entry").getAsString();exportArgs.add(address);if(toAddr(address).equals(entry))included=true;}
        if(!included || exportArgs.size()>33)throw new IllegalArgumentException("Selected entry must belong to the bounded seed");
        runScript("ExportRecompilationSeed.java",exportArgs.toArray(new String[0]));
        var seed=JsonParser.parseString(Files.readString(output)).getAsJsonObject();var recovery=new JsonObject();
        recovery.addProperty("kind","prolog-linked-envelope-code");recovery.addProperty("function",entry.toString());recovery.addProperty("table",table.toString());
        recovery.addProperty("recordCount",recordCount);recovery.addProperty("gapBytes",gapBytes);recovery.add("records",records);recovery.addProperty("runtimeObserved",false);
        recovery.addProperty("priorSeed",input.toString());seed.add("layoutRecovery",recovery);Files.writeString(output,seed.toString());
        println("Linked cold code exported inside the original envelope; function membership unchanged");
    }
}
