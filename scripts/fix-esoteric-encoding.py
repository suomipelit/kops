import sys

for fn in sys.argv[1:]:
	with open(fn, 'rb') as infp:
		data = infp.read()
	out = []
	for c in data:
		if c > 127:
			out.append('\\%03o' % c)
		else:
			out.append(chr(c))
	out = ''.join(out)
	if out != data.decode('utf-8', 'replace'):
		print(fn)
		with open(fn, 'w', encoding='ascii') as outfp:
			outfp.write(out)
