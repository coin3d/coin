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
parser.add_argument('--shared-binary', help='CoinSharedCoordinateBenchmark for 400-shape scenes')
args = parser.parse_args()
libraries = [('before', args.before_library_dir), ('after', args.after_library_dir)]
results = []
for mode, side in [('texture', 100), ('texture', 200), ('texture', 400),
                   ('explicit', 200), ('no-texture', 200)]:
    for repeat in range(3):
        order = libraries if repeat % 2 == 0 else list(reversed(libraries))
        for name, libdir in order:
            env = dict(os.environ, LD_LIBRARY_PATH=libdir,
                       COIN_GLX_PIXMAP_DIRECT_RENDERING='1',
                       COIN_VERTEX_ARRAYS='1', COIN_VBO='1')
            process = subprocess.run([args.binary, mode, str(side), '30'],
                                     env=env, text=True, capture_output=True, check=True)
            row = json.loads(process.stdout)
            if row['valid'] != 1:
                raise RuntimeError('Invalid rendered image')
            row.update(library=name, repeat=repeat, mode=mode, renderer=process.stderr.strip())
            results.append(row)
            print(json.dumps(row), flush=True)
summary = []
for mode, side in sorted({(row['mode'], row['side']) for row in results}):
    rows = [r for r in results if r['mode'] == mode and r['side'] == side]
    if len({r['hash'] for r in rows}) != 1:
        raise RuntimeError('Different rendered images between libraries or repetitions')
    fields = ['median_ms', 'first_ms', 'invalidated_ms', 'restabilized_ms',
              'rss_scene_mib', 'rss_first_mib', 'rss_steady_mib', 'rss_invalidated_mib',
              'rss_restabilized_mib', 'rss_after_release_mib', 'peak_mib']
    metrics = {name: {key: statistics.median(r[key] for r in rows if r['library'] == name)
                      for key in fields} for name, _ in libraries}
    summary.append(dict(mode=mode, faces=side*side, **metrics,
                        reduction_percent=100*(1-metrics['after']['median_ms']/metrics['before']['median_ms'])))
shared_runs = []
if args.shared_binary:
    for layout in ['shared', 'own']:
        for repeat in range(3):
            for name, libdir in (libraries if repeat % 2 == 0 else list(reversed(libraries))):
                env = dict(os.environ, LD_LIBRARY_PATH=libdir, COIN_VERTEX_ARRAYS='1',
                           COIN_VBO='1', COIN_GLX_PIXMAP_DIRECT_RENDERING='1',
                           COIN_BENCHMARK_SHARED_POINTS='410881', COIN_BENCHMARK_VERTICES_PER_SHAPE='4096')
                p = subprocess.run([args.shared_binary, layout, 'face', 'explicit', 'uv',
                                    '2d', '400', '10', 'coord'], env=env, text=True,
                                   capture_output=True, check=True)
                phases = [json.loads(line) for line in p.stdout.splitlines()]
                for row in phases:
                    if row.get('missing', 0) or ('nonzero_bytes' in row and not row['nonzero_bytes']):
                        raise RuntimeError('Invalid shared benchmark image/coordinate state')
                shared_runs.append(dict(layout=layout, library=name, repeat=repeat,
                                        phases=phases, renderer=p.stderr.strip()))
                print(json.dumps(shared_runs[-1]), flush=True)
    for layout in ['shared', 'own']:
        for phase in ['first', 'static', 'invalidated', 'restabilized']:
            hashes = {p['hash'] for r in shared_runs if r['layout'] == layout
                      for p in r['phases'] if p['phase'] == phase}
            if len(hashes) != 1:
                raise RuntimeError('Different shared benchmark images between libraries')
Path(args.output).write_text(json.dumps(dict(runs=results, summary=summary,
                                           shared_runs=shared_runs), indent=2)+'\n')
print('SUMMARY '+json.dumps(summary), flush=True)
