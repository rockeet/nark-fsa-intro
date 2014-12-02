#define _SCL_SECURE_NO_WARNINGS
#include <nark/fsa/forward_decl.hpp>
#include <nark/fsa/mre_match.hpp>
#include <nark/util/linebuf.hpp>
#include <nark/util/autoclose.hpp>
#include <nark/util/profiling.hpp>
#include <malloc.h>

using namespace nark;

bool read_one_record(FILE* fp, nark::LineBuf* line, bool binary) {
	if (binary) {
		int32_t offsets[2];
		return line->read_binary_tuple(offsets, 1, fp);
	}
	else {
		bool ret =  line->getline(fp) > 0;
		line->chomp();
		return ret;
	}
}

int main(int argc, char* argv[]) {
	const char* dfa_file = NULL;
	const char* txt_file = NULL;
	bool verbose = false;
	bool binary = false;
	bool ignore_case = false;
	for (int opt=0; (opt = getopt(argc, argv, "f:i:vBI")) != -1; ) {
		switch (opt) {
			case '?': return 1;
			case 'i': dfa_file = optarg; break;
			case 'f': txt_file = optarg; break;
			case 'v': verbose  = true;   break;
			case 'B': binary   = true;   break;
			case 'I': ignore_case = true;break;
		}
	}
	if (NULL == dfa_file) {
		fprintf(stderr, "usage: %s -i dfa_file [-f match_file] [-v]\n", argv[0]);
		return 1;
	}
	nark::Auto_fclose fp;
	if (txt_file) {
		fp = fopen(txt_file, "r");
		if (NULL == fp) {
			fprintf(stderr, "fopen(%s, r) = %m\n", txt_file);
			return 1;
		}
	}
	DFA_InterfacePtr dfa(DFA_Interface_load(dfa_file));
	MultiRegexFullMatch fm(dfa.get());
	nark::profiling pf;
	long ts = pf.now();
	fm.warm_up();
	nark::LineBuf line;
	long t0 = pf.now();
	long lineno = 0;
	long sumlen = 0;
	long matched = 0;
	while (read_one_record(fp.self_or(stdin), &line, binary)) {
		lineno++;
		long len;
		if (ignore_case)
			 len = fm.match(fstring(line.p, line.n), ::tolower);
		else len = fm.match(fstring(line.p, line.n));
		if (verbose && fm.size()) {
			printf("line:%ld:len=%ld:", lineno, len);
			for (size_t i = 0; i < fm.size(); ++i) {
				printf(" %d", fm[i]);
			}
			printf("\n");
		}
		sumlen += line.n;
		if (fm.size())
			matched++;
	}
	long t1 = pf.now();
	printf("time(warm_up)=%f's\n", pf.sf(ts, t0));
	printf("time=%f's lines=%ld matched=%ld QPS=%f Throughput=%f'MiB Latency=%f'us\n"
			, pf.sf(t0,t1)
			, lineno
			, matched
			, lineno/pf.sf(t0,t1)
			, sumlen/pf.uf(t0,t1)
			, pf.uf(t0,t1)/lineno
			);
	malloc_stats();
	return 0;
}

