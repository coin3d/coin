#!/usr/bin/env python3
"""Compare two Coin libraries serially in one Xvfb session; emit JSON results."""
import argparse
import json
import os
import statistics
import subprocess
from pathlib import Path

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('binary')
parser.add_argument('before_library_dir')
parser.add_argument('after_library_dir')
parser.add_argument('--output', required=True)
args = parser.parse_args()
libraries = [('before', args.before_library_dir), ('after', args.after_library_dir)]
results = []
for side in [100, 200, 400]:
    for repeat in range(3):
        order = libraries if repeat % 2 == 0 else list(reversed(libraries))
        for name, libdir in order:
            env = dict(os.environ, LD_LIBRARY_PATH=libdir,
                       COIN_GLX_PIXMAP_DIRECT_RENDERING='1',
                       COIN_VERTEX_ARRAYS='1', COIN_VBO='1')
            process = subprocess.run([args.binary, 'texture', str(side), '30'],
                                     env=env, text=True, capture_output=True, check=True)
            row = json.loads(process.stdout)
            if row['valid'] != 1:
                raise RuntimeError('Invalid rendered image')
            row.update(library=name, repeat=repeat, renderer=process.stderr.strip())
            results.append(row)
            print(json.dumps(row), flush=True)
summary = []
for side in [100, 200, 400]:
    times = {name: statistics.median(row['median_ms'] for row in results
             if row['side'] == side and row['library'] == name) for name, _ in libraries}
    summary.append(dict(faces=side*side, **times,
                        speedup=times['before']/times['after'],
                        reduction_percent=100*(1-times['after']/times['before'])))
Path(args.output).write_text(json.dumps(dict(runs=results, summary=summary), indent=2)+'\n')
print('SUMMARY '+json.dumps(summary), flush=True)
