// Focused symbol/reference discovery; derived output belongs in ignored research.
import ghidra.app.script.GhidraScript;
import java.nio.file.Files;
import java.nio.file.Path;
public class FindRenderStateRefs extends GhidraScript {
  @Override public void run() throws Exception {
    var args=getScriptArgs();
    if(args.length<1||args.length>9)throw new IllegalArgumentException("Output and up to eight optional addresses required");
    var out=new StringBuilder(); int matched=0;
    if(args.length>1){
      for(int i=1;i<args.length;i++){
        var address=toAddr(args[i]);out.append("ADDRESS ").append(address).append("\n");int count=0;
        for(var ref:getReferencesTo(address)){
          if(++count>64){out.append("reference limit reached\n");break;}
          var caller=getFunctionContaining(ref.getFromAddress());
          out.append(" ref ").append(ref.getFromAddress()).append(" caller ").append(caller==null?"unknown":caller.getEntryPoint()).append("\n");
        }
      }
      Files.writeString(Path.of(args[0]),out.toString());return;
    }
    var functions=currentProgram.getFunctionManager().getFunctions(true);
    while(functions.hasNext()&&!monitor.isCancelled()) {
      var f=functions.next(); var name=f.getName(true);
      if(!name.matches("(?i).*(alpha|blend|setvertexshader|setstreamsource|drawvertices).*"))continue;
      if(++matched>128){out.append("symbol limit reached\n");break;}
      out.append(f.getEntryPoint()).append(" ").append(name).append("\n");
      int count=0;
      for(var ref:getReferencesTo(f.getEntryPoint())) {
        if(++count>64){out.append("reference limit reached\n");break;}
        var caller=getFunctionContaining(ref.getFromAddress());
        out.append(" ref ").append(ref.getFromAddress()).append(" caller ")
          .append(caller==null?"unknown":caller.getEntryPoint()).append("\n");
      }
    }
    Files.writeString(Path.of(args[0]),out.toString());
    println("Render-state discovery saved locally");
  }
}
