#!/usr/bin/env python3
import argparse
import os
import glob
import hashlib
import subprocess

clang_tidy = os.environ.get('CLANG_TIDY', '/usr/local/opt/llvm/bin/clang-tidy')
clang_format = os.environ.get('CLANG_FORMAT', '/usr/local/opt/llvm/bin/clang-format')


def get_checksum(path):
	with open(path, 'rb') as infp:
		return hashlib.sha256(infp.read()).hexdigest()

def prettify_target(target):
    if not (target.endswith('.c') or target.endswith('.h')):
        return None

    pre_tidy_checksum = get_checksum(target)
    print('-' * 40)
    print(target)
    print('-' * 40)
    print('\n')
    subprocess.call([
        clang_tidy,
        '-fix',
        '-fix-errors',
        '-p', 'cmake-build-debug/',
        target,
    ])
    subprocess.check_call([
        clang_format,
        '-i',
        target,
    ])
    post_tidy_checksum = get_checksum(target)
    if pre_tidy_checksum != post_tidy_checksum:
        print('*** CHANGED: %s' % target)
        return True
    return False

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--all', action='store_true', default=False)
    ap.add_argument('targets', nargs='*')
    args = ap.parse_args()
    if args.all:
        args.targets = sorted(glob.glob('src/**', recursive=True))
    for target in args.targets:
        prettify_target(target)

if __name__ == '__main__':
    main()