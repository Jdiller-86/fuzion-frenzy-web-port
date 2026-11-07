"""Bounded replacement for pinned-lifter RDTSC helper calls."""

import re


_ADDRESS = re.compile(r"^[0-9A-Fa-f]{1,8}$")
_IDENTIFIER = re.compile(r"\b[A-Za-z_]\w*\b")
_READ_CALL = re.compile(r"\bxbox_ReadTimeStampCounter\(\)")
_HELPER_LIKE = re.compile(r"(?i)(?:probe.*(?:tsc|timestamp)|(?:tsc|timestamp).*probe|xbox_.*(?:timestamp|tsc))")


def guard_timestamp_reads(text, rows):
    """Replace each checked RDTSC helper call with its original instruction site.

    ``rows`` contains checked Ghidra row dictionaries.  This function validates
    the row/call correspondence and changes only the helper-call byte spans.
    """
    sites = []
    seen = set()
    previous = None
    for row in rows:
        if str(row.get("mnemonic", "")).upper() != "RDTSC":
            continue
        address_text = row.get("address")
        length = row.get("length")
        opcode = row.get("analysisBytesHex")
        if not isinstance(address_text, str) or not _ADDRESS.fullmatch(address_text):
            raise ValueError("RDTSC row has a noncanonical address")
        address = int(address_text, 16)
        if address > 0xFFFFFFFE:
            raise ValueError("RDTSC instruction crosses the 32-bit address boundary")
        if address in seen:
            raise ValueError("duplicate RDTSC row address")
        if previous is not None and address <= previous:
            raise ValueError("RDTSC rows are not in strictly increasing address order")
        seen.add(address)
        previous = address
        if type(length) is not int or length != 2:
            raise ValueError("RDTSC row must have length two")
        if not isinstance(opcode, str) or opcode.lower() != "0f31":
            raise ValueError("RDTSC row must contain opcode 0f31")
        sites.append(address)

    identifiers = [m.group(0) for m in _IDENTIFIER.finditer(text)]
    helper_ids = [name for name in identifiers if _HELPER_LIKE.search(name)]
    calls = list(_READ_CALL.finditer(text))
    if len(helper_ids) != len(calls):
        raise ValueError("malformed or altered timestamp helper reference")
    if len(calls) != len(sites):
        raise ValueError("timestamp helper calls do not match checked RDTSC rows")
    if not sites:
        return text, []

    pieces = []
    cursor = 0
    for match, address in zip(calls, sites):
        pieces.append(text[cursor:match.start()])
        pieces.append(f"ff_probe_read_tsc(0x{address:08X}u)")
        cursor = match.end()
    pieces.append(text[cursor:])
    return "".join(pieces), sites
