// Static loader/decompiler verification. The imported target is never executed.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import com.google.gson.JsonObject;
import java.nio.file.Files;
import java.nio.file.Path;

public class VerifyXbeSetup extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("Expected output JSON path");
        JsonObject result = new JsonObject();
        result.addProperty("verification", "static Xbox loader and entry decompilation");
        result.addProperty("executableFormat", currentProgram.getExecutableFormat());
        result.addProperty("language", currentProgram.getLanguageID().toString());
        result.addProperty("sourceSha256", currentProgram.getExecutableSHA256());
        result.addProperty("memoryBlocks", currentProgram.getMemory().getBlocks().length);
        var entries = currentProgram.getSymbolTable().getExternalEntryPointIterator();
        if (!entries.hasNext()) throw new IllegalStateException("Loader established no entry point");
        Address entry = entries.next();
        result.addProperty("entry", entry.toString());
        disassemble(entry);
        Function function = currentProgram.getFunctionManager().getFunctionAt(entry);
        if (function == null) function = createFunction(entry, "setup_verification_entry");
        if (function == null) throw new IllegalStateException("Could not create entry function");
        DecompInterface decompiler = new DecompInterface();
        try {
            if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
            DecompileResults decompiled = decompiler.decompileFunction(function, 30, monitor);
            result.addProperty("decompileCompleted", decompiled.decompileCompleted());
            result.addProperty("error", decompiled.getErrorMessage());
            if (decompiled.getDecompiledFunction() != null) result.addProperty("entryPseudocode", decompiled.getDecompiledFunction().getC());
        } finally { decompiler.dispose(); }
        Files.writeString(Path.of(args[0]), result.toString());
        println("PROJECT_XBE_SETUP_RESULT=" + result.toString());
        if (!result.get("decompileCompleted").getAsBoolean()) throw new IllegalStateException("Entry decompilation failed");
    }
}
