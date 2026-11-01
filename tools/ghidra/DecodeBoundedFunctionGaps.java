// Decode only small undefined gaps in explicitly selected function envelopes.
// Use in a -readOnly session; no original bytes or persistent project are changed.
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.AddressSet;

public class DecodeBoundedFunctionGaps extends GhidraScript {
    @Override public void run() throws Exception {
        var args = getScriptArgs();
        if (args.length < 1 || args.length > 8) throw new IllegalArgumentException("Select 1-8 established functions");
        int decoded = 0;
        for (var argument : args) {
            var function = getFunctionAt(toAddr(argument));
            if (function == null || function.isExternal() || function.isThunk()) {
                throw new IllegalArgumentException("Function is not established locally");
            }
            var cursor = function.getBody().getMinAddress();
            var last = function.getBody().getMaxAddress();
            if (!cursor.equals(function.getEntryPoint()) || last.subtract(cursor) >= 65536) {
                throw new IllegalArgumentException("Unsupported envelope");
            }
            while (cursor.compareTo(last) <= 0) {
                if (monitor.isCancelled()) throw new IllegalStateException("Cancelled");
                var instruction = currentProgram.getListing().getInstructionAt(cursor);
                if (instruction == null) {
                    var gapLast = cursor;
                    while (gapLast.compareTo(last) <= 0 && currentProgram.getListing().getInstructionAt(gapLast) == null) {
                        if (currentProgram.getListing().getDefinedDataContaining(gapLast) != null ||
                            currentProgram.getListing().getInstructionContaining(gapLast) != null ||
                            gapLast.subtract(cursor) + 1 > 256 - decoded) {
                            throw new IllegalArgumentException("Defined data or gap budget exceeded");
                        }
                        gapLast = gapLast.add(1);
                    }
                    gapLast = gapLast.subtract(1);
                    var command = new DisassembleCommand(cursor, new AddressSet(cursor, gapLast), false);
                    command.enableCodeAnalysis(false);
                    if (!command.applyTo(currentProgram, monitor)) throw new IllegalStateException(command.getStatusMsg());
                    instruction = currentProgram.getListing().getInstructionAt(cursor);
                    if (instruction == null) throw new IllegalStateException("Gap remains undecoded");
                    decoded += (int)command.getDisassembledAddressSet().getNumAddresses();
                    if (decoded > 256) throw new IllegalStateException("Disassembler exceeded the gap budget");
                }
                if (instruction.getMaxAddress().compareTo(last) > 0) throw new IllegalStateException("Instruction crosses envelope");
                cursor = instruction.getMaxAddress().add(1);
            }
        }
        println("Bounded gap bytes decoded in analysis memory: " + decoded);
    }
}
