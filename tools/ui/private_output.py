"""Output containment checks for the local Pillow artifact generators."""
from pathlib import Path
import subprocess


def no_links(value):
    path = Path(value).absolute()
    for ancestor in reversed([path, *path.parents]):
        if ancestor.is_symlink():
            raise ValueError(f'Links are not allowed in artifact paths: {ancestor}')
    return path


def private_directory(value, inputs):
    out = no_links(value)
    for value in inputs:
        source = no_links(value)
        if out == source or (source.is_dir() and source in out.parents) or out in source.parents:
            raise ValueError('Output overlaps a supplied input')
    repo = next((parent for parent in [out, *out.parents] if (parent / '.git').exists()), None)
    if repo is None or subprocess.run(['git', '-C', str(repo), 'check-ignore', '--no-index', '--quiet', '--', str(out)], capture_output=True).returncode:
        raise ValueError('Private output must be inside a Git-ignored project directory')
    out.mkdir(parents=True, exist_ok=True)
    return no_links(out)
