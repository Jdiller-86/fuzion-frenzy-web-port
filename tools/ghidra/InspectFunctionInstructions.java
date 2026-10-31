// Bounded disassembly for checking decompiler calling-convention uncertainty.
import ghidra.app.script.GhidraScript;
import java.nio.file.Files;
import java.nio.file.Path;
public class InspectFunctionInstructions extends GhidraScript {
  @Override public void run() throws Exception {
    var args=getScriptArgs();
    if(args.length<2||args.length>9)throw new IllegalArgumentException("Output and 1-8 function addresses required");
    var out=new StringBuilder();
    for(int i=1;i<args.length;i++) {
      var f=getFunctionAt(toAddr(args[i]));
      if(f==null)throw new IllegalArgumentException("Function not established: "+args[i]);
      out.append("FUNCTION ").append(f.getEntryPoint()).append("\n");
      var instructions=currentProgram.getListing().getInstructions(f.getBody(),true); int count=0;
      while(instructions.hasNext()&&!monitor.isCancelled()) {
        if(++count>2048){out.append("instruction limit reached\n");break;}
        var instruction=instructions.next();
        out.append(instruction.getAddress()).append(" ").append(instruction).append("\n");
      }
    }
    Files.writeString(Path.of(args[0]),out.toString());
    println("Selected disassembly saved locally");
  }
}
