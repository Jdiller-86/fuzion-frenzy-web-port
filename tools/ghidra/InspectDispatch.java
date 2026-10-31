import ghidra.app.script.GhidraScript;
import java.nio.file.Files;
import java.nio.file.Path;
public class InspectDispatch extends GhidraScript {
  @Override public void run() throws Exception {
    var args=getScriptArgs(); if(args.length!=1) throw new IllegalArgumentException();
    var a=toAddr("002852e8"); var b=new StringBuilder();
    for(int i=0;i<8;i++) b.append(i).append(" ").append(String.format("%08x", currentProgram.getMemory().getInt(a.add(i*4)))).append("\n");
    Files.writeString(Path.of(args[0]), b.toString());
  }
}
