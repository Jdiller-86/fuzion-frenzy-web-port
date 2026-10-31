// Read a bounded list of static 32-bit words into an ignored local evidence file.
import ghidra.app.script.GhidraScript;
import java.nio.file.Files;
import java.nio.file.Path;
public class InspectStaticWords extends GhidraScript {
  @Override public void run() throws Exception {
    var args=getScriptArgs(); if(args.length!=3) throw new IllegalArgumentException("output, address, word count required");
    int count=Integer.parseInt(args[2]); if(count<1||count>64)throw new IllegalArgumentException("word count outside bound");
    var address=toAddr(args[1]); var output=new StringBuilder();
    for(int i=0;i<count;i++)output.append(address.add(i*4)).append(" ").append(String.format("%08x",currentProgram.getMemory().getInt(address.add(i*4)))).append("\n");
    Files.writeString(Path.of(args[0]),output.toString());
  }
}
