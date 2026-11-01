// Read-only, bounded function evidence for a local recompilation proof.
// Original instruction bytes/text are written only to a new ignored research file.
import ghidra.app.script.GhidraScript;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

public class ExportRecompilationSeed extends GhidraScript {
    @Override public void run() throws Exception {
        var args = getScriptArgs();
        if (args.length < 2 || args.length > 9) {
            throw new IllegalArgumentException("Output and 1-8 exact function entries required");
        }
        var project = Path.of(getSourceFile().getAbsolutePath()).toRealPath()
                .getParent().getParent().getParent();
        var research = project.resolve("research").toRealPath();
        var output = Path.of(args[0]).toAbsolutePath().normalize();
        var parent = output.getParent().toRealPath();
        if (!parent.startsWith(research) || !output.toString().endsWith(".json") || Files.exists(output)) {
            throw new IllegalArgumentException("Use a new JSON output inside local research");
        }
        var result = new JsonObject();
        result.addProperty("kind", "bounded-ghidra-recompilation-seed");
        result.addProperty("sourceSha256", currentProgram.getExecutableSHA256());
        result.addProperty("format", currentProgram.getExecutableFormat());
        result.addProperty("language", currentProgram.getLanguageID().toString());
        result.addProperty("runtimeObserved", false);
        var functions = new JsonArray();
        int total = 0;
        for (int i = 1; i < args.length; ++i) {
            var address = toAddr(args[i]);
            var item = new JsonObject();
            item.addProperty("requested", address.toString());
            var function = getFunctionAt(address);
            item.addProperty("established", function != null);
            if (function == null) {
                functions.add(item);
                continue;
            }
            item.addProperty("entry", function.getEntryPoint().toString());
            item.addProperty("name", function.getName());
            item.addProperty("external", function.isExternal());
            item.addProperty("thunk", function.isThunk());
            item.addProperty("bodyBytes", function.getBody().getNumAddresses());
            var ranges = new JsonArray();
            var iterator = function.getBody().getAddressRanges();
            while (iterator.hasNext()) {
                var range = iterator.next();
                var r = new JsonObject();
                r.addProperty("start", range.getMinAddress().toString());
                r.addProperty("endInclusive", range.getMaxAddress().toString());
                ranges.add(r);
            }
            item.add("bodyRanges", ranges);
            var instructions = new JsonArray();
            var listing = currentProgram.getListing().getInstructions(function.getBody(), true);
            int count = 0;
            while (listing.hasNext()) {
                if (monitor.isCancelled()) throw new IllegalStateException("Cancelled");
                if (++count > 4096 || ++total > 8192) throw new IllegalArgumentException("Instruction budget exceeded");
                var instruction = listing.next();
                var row = new JsonObject();
                row.addProperty("address", instruction.getAddress().toString());
                row.addProperty("length", instruction.getLength());
                row.addProperty("mnemonic", instruction.getMnemonicString());
                row.addProperty("text", instruction.toString());
                var hex = new StringBuilder();
                for (byte b : instruction.getBytes()) hex.append(String.format("%02x", b & 255));
                row.addProperty("analysisBytesHex", hex.toString());
                row.addProperty("call", instruction.getFlowType().isCall());
                row.addProperty("computed", instruction.getFlowType().isComputed());
                if (instruction.getFallThrough() != null) {
                    row.addProperty("fallthrough", instruction.getFallThrough().toString());
                }
                var flows = new JsonArray();
                for (var target : instruction.getFlows()) flows.add(target.toString());
                row.add("flows", flows);
                var references = new JsonArray();
                for (var reference : instruction.getReferencesFrom()) {
                    var ref = new JsonObject();
                    ref.addProperty("target", reference.getToAddress().toString());
                    ref.addProperty("type", reference.getReferenceType().toString());
                    references.add(ref);
                }
                row.add("references", references);
                instructions.add(row);
            }
            item.add("instructions", instructions);
            functions.add(item);
        }
        result.add("functions", functions);
        Files.writeString(output, result.toString(), StandardOpenOption.CREATE_NEW);
        println("Bounded recompilation seed saved locally; instructions=" + total);
    }
}
