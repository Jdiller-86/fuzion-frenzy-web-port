// Writes narrowly selected static decompilations to an ignored local path.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import java.nio.file.Files;
import java.nio.file.Path;

public class InspectResourceCalls extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    if (args.length < 2) throw new IllegalArgumentException("Expected local path and function addresses");
    StringBuilder out = new StringBuilder();
    DecompInterface d = new DecompInterface();
    try {
      if (!d.openProgram(currentProgram)) throw new IllegalStateException(d.getLastMessage());
      for (int i=1;i<args.length;i++) {
        var address = toAddr(args[i]);
        var f = getFunctionAt(address);
        if (f == null) f = getFunctionContaining(address);
        if (f == null) { disassemble(address); f = createFunction(address, null); }
        if (f == null) { out.append(args[i]).append(" no function\n"); continue; }
        var result = d.decompileFunction(f, 30, monitor);
        out.append("FUNCTION ").append(f.getEntryPoint()).append(" complete=").append(result.decompileCompleted()).append("\n");
        if (result.getDecompiledFunction() != null) out.append(result.getDecompiledFunction().getC()).append("\n");
      }
    } finally { d.dispose(); }
    Files.writeString(Path.of(args[0]), out.toString());
    println("Selected output saved to local research directory");
  }
}
