// Focused static lookup in the existing analysis project; output stays local.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.Reference;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class FindResourceRefs extends GhidraScript {
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("Expected local output path");
        StringBuilder out = new StringBuilder();
        String[] terms = {"characters\\dub.obe", "Twisted System", "%s.obe"};
        for (String term : terms) {
            byte[] bytes = term.getBytes(StandardCharsets.US_ASCII);
            Address from = currentProgram.getMinAddress();
            while (from != null) {
                Address found = currentProgram.getMemory().findBytes(from, bytes, null, true, monitor);
                if (found == null) break;
                out.append("match ").append(term).append(" ").append(found).append("\n");
                for (Reference ref : getReferencesTo(found)) {
                    var f = getFunctionContaining(ref.getFromAddress());
                    out.append("  ref ").append(ref.getFromAddress()).append(" function ").append(f == null ? "none" : f.getEntryPoint()).append("\n");
                }
                from = found.next();
            }
        }
        Files.writeString(Path.of(args[0]), out.toString());
        println("Resource-reference lookup saved to local research directory");
    }
}
