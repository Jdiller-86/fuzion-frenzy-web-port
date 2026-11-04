// Bounded static closure for an actually reached startup root. This does not
// execute original code, establish runtime reachability or change the project.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashSet;

public class ExportRecompilationClosure extends GhidraScript {
    @Override public void run() throws Exception {
        var args=getScriptArgs();
        if(args.length<3 || args.length>8)throw new IllegalArgumentException("New output, admitted generation and 1-6 roots required");
        var project=Path.of(getSourceFile().getAbsolutePath()).toRealPath().getParent().getParent().getParent();
        var research=project.resolve("research").toRealPath();
        var output=Path.of(args[0]).toAbsolutePath().normalize();
        var priorPath=Path.of(args[1]).toRealPath();
        if(!output.getParent().toRealPath().startsWith(research) || !priorPath.startsWith(research) || Files.exists(output) ||
           !output.toString().endsWith(".json") || Files.size(priorPath)>1024*1024)throw new IllegalArgumentException("Use bounded private research paths and preserve existing output");
        var prior=JsonParser.parseString(Files.readString(priorPath)).getAsJsonObject();
        if(!prior.get("sourceSha256").getAsString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalArgumentException("Prior executable identity differs");
        var known=new HashSet<Address>();
        for(var item:prior.getAsJsonArray("functions"))known.add(toAddr(Long.toHexString(item.getAsJsonObject().get("entry").getAsLong())));
        int limit=prior.has("functionLimit")?prior.get("functionLimit").getAsInt():64;
        if(limit!=64 && limit!=128 && limit!=256 && limit!=512)throw new IllegalArgumentException("Unsupported explicit function limit");
        int instructionLimit=prior.has("instructionLimit")?prior.get("instructionLimit").getAsInt():8192;
        if(instructionLimit!=8192 && instructionLimit!=16384)throw new IllegalArgumentException("Unsupported explicit instruction budget");
        int functionBudget=Math.min(limit>=256?32:8,limit-known.size()),instructionBudget=Math.min(8192,instructionLimit-prior.get("instructions").getAsInt());
        if(functionBudget<1 || instructionBudget<1)throw new IllegalArgumentException("No remaining aggregate admission budget");
        var queue=new ArrayDeque<Address>();var discovered=new HashSet<Address>();var rootAddresses=new HashSet<Address>();
        for(int i=2;i<args.length;++i) {var address=toAddr(args[i]);rootAddresses.add(address);if(discovered.add(address))queue.add(address);}
        var selected=new ArrayList<String>();var unresolved=new JsonArray();int instructions=0;
        while(!queue.isEmpty() && selected.size()<functionBudget) {
            var address=queue.remove();boolean admitted=known.contains(address);
            if(admitted && !rootAddresses.contains(address))continue;
            var function=getFunctionAt(address);
            if(function==null || function.isExternal()) {unresolved.add(address.toString());continue;}
            var rows=currentProgram.getListing().getInstructions(function.getBody(),true);int count=0;
            while(rows.hasNext()) {rows.next();count++;}
            if(count>4096 || function.getBody().getNumAddresses()>65536 || !admitted && instructions+count>instructionBudget) {queue.addFirst(address);break;}
            if(!admitted) {selected.add(address.toString());instructions+=count;}
            rows=currentProgram.getListing().getInstructions(function.getBody(),true);
            while(rows.hasNext()) {
                var instruction=rows.next();var flow=instruction.getFlowType();
                var targets=new ArrayList<Address>();
                if(flow.isCall() || flow.isJump())for(var target:instruction.getFlows())targets.add(target);
                // Constructors also pass code pointers to shared iterator code.
                // Admit only already established executable function entries;
                // these references are static candidates, never observed calls.
                for(var reference:instruction.getReferencesFrom()) {
                    var target=reference.getToAddress();var block=currentProgram.getMemory().getBlock(target);
                    if(reference.getReferenceType().isData() && block!=null && block.isExecute() && getFunctionAt(target)!=null)targets.add(target);
                }
                for(var target:targets) {
                    if(function.getBody().contains(target) || known.contains(target) || !discovered.add(target))continue;
                    if(discovered.size()>64)throw new IllegalArgumentException("Static frontier exceeds bounded discovery budget");
                    queue.add(target);
                }
            }
        }
        if(selected.isEmpty())throw new IllegalArgumentException("No established local function selected");
        var exportArgs=new ArrayList<String>();exportArgs.add(output.toString());exportArgs.addAll(selected);
        runScript("ExportRecompilationSeed.java",exportArgs.toArray(new String[0]));
        JsonObject seed=JsonParser.parseString(Files.readString(output)).getAsJsonObject();
        var selection=new JsonObject();selection.addProperty("kind","bounded-static-call-closure");selection.addProperty("priorGeneration",priorPath.toString());
        selection.addProperty("includesEstablishedCodeReferences",true);
        selection.addProperty("instructions",instructions);selection.addProperty("runtimeObserved",false);
        var roots=new JsonArray();for(int i=2;i<args.length;++i)roots.add(args[i]);selection.add("roots",roots);
        var frontier=new JsonArray();for(var address:queue)if(!known.contains(address))frontier.add(address.toString());
        selection.add("remainingFrontier",frontier);selection.add("unresolved",unresolved);seed.add("selection",selection);
        Files.writeString(output,seed.toString());
        println("Bounded static closure exported locally: functions="+selected.size()+", instructions="+instructions+", frontier="+frontier.size());
    }
}
