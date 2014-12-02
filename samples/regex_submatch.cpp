#define _SCL_SECURE_NO_WARNINGS
#include <nark/fsa/forward_decl.hpp>
#include <nark/fsa/mre_match.hpp>
#include <nark/util/linebuf.hpp>
#include <nark/util/profiling.hpp>
#include <getopt.h>
#include <malloc.h>

using namespace nark;

void match_and_print(MultiRegexSubmatch& sub, fstring text) {
	printf("%s ----------\n", text.p);
	int max_match_len = sub.match_utf8(text, ::tolower);
	for(int j = 0; j < (int)sub.fullmatch_regex().size(); ++j) {
		int regex_id = sub.fullmatch_regex()[j];
		int nsub = sub.num_submatch(regex_id);
		for(int k = 0; k < nsub; ++k) {
			fstring str = sub(text.p, regex_id, k);
			printf("j=%d regex_id=%d sub(%d): %.*s\n", j, regex_id, k, str.ilen(), str.data());
		}
	}
	printf("max_match_len=%d: %.*s\n", max_match_len, max_match_len, text.p);
}

int main(int argc, char* argv[]) {
	const char* bin_meta_file = NULL;
	const char* dfa_file = NULL;
	bool verbose = false;
	for (int opt=0; (opt = getopt(argc, argv, "b:i:v")) != -1; ) {
		switch (opt) {
		case '?': return 1;
		case 'b': bin_meta_file = optarg;  break;
		case 'i': dfa_file = optarg;       break;
		case 'v': verbose  = true;         break;
		}
	}
	if (NULL == bin_meta_file) {
		fprintf(stderr, "-b bin_meta_file is required\n");
		return 1;
	}
	if (NULL == dfa_file) {
		fprintf(stderr, "loading dfa file from stdin...\n");
		DFA_InterfacePtr dfa(DFA_Interface_load(stdin));
		MultiRegexSubmatch sub(&*dfa, bin_meta_file);
		for (int i = optind; i < argc; ++i) {
			match_and_print(sub, argv[i]);
		}
	}
	else {
		DFA_InterfacePtr dfa(DFA_Interface_load(dfa_file));
		MultiRegexSubmatch sub(&*dfa, bin_meta_file);
		nark::LineBuf line;
		nark::profiling pf;
		long t0 = pf.now();
		long lineno = 0;
		while (line.getline(stdin) > 0) {
			lineno++;
			line.chomp();
			if (verbose)
				match_and_print(sub, fstring(line.p, line.n));
			else
				sub.match_utf8(fstring(line.p, line.n), ::tolower);
		}
		long t1 = pf.now();
		printf("time=%f's QPS=%f Latency=%f'us\n"
			, pf.sf(t0,t1)
			, lineno/pf.sf(t0,t1)
			, pf.uf(t0,t1)/lineno
			);
	}
	return 0;
}

