"""
Generate Samples.dox RAII_Samples.dox
"""

import argparse
from glob import glob
from os.path import isfile, join, basename
import sys


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('-s', '--sample-folder', help="Sample folder", default="samples")
    parser.add_argument('-o', '--output-file', help="Output file", default="docs/Samples.dox")
    args = parser.parse_args()

    with open(args.output_file, 'w') as f:
        f.write(fr"""/**
\cond
File generate using `python3 {' '.join(sys.argv)}`
\endcond
\page {basename(args.output_file).replace('.dox', '')} {basename(args.output_file).replace('.dox', '').replace('_', ' ')}
""")  # noqa
        pages = []
        for sample in sorted(glob(join(args.sample_folder, '*'))):
            file = join(sample, basename(sample) + '.cpp')
            if isfile(file):
                pages.append({
                    'file': file,
                    'page': basename(args.output_file).replace('.dox', '') + '_' + basename(sample),
                    'title': basename(sample),
                })
        for page in pages:
            f.write(f"""\\subpage {page['page']}\n\n""")

        for page in pages:
            f.write(rf"""
\page {page['page']} {page['title']}
\include {page['file']}
""")
        f.write(r"*/")
    with open(args.output_file) as f:
        print(f.read())


if __name__ == '__main__':
    main()
