#define _SCL_SECURE_NO_WARNINGS
#include <nark/fsa/pinyin_dfa.hpp>
#include <nark/fsa/forward_decl.hpp>
#include <nark/util/linebuf.hpp>
#include <nark/util/profiling.hpp>

using namespace nark;

bool be_quiet = false;
void OnMatch(int klen, int, fstring value) {
	if (!be_quiet)
		printf("%d\t%.*s\n", klen, value.ilen(), value.data());
}
int main(int argc, char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "usage: %s dfa_file  base_pinyin_text_file\n", argv[0]);
		return 1;
	}
	if (char* env = getenv("be_quiet")) {
		be_quiet = atoi(env);
	}
	DFA_InterfacePtr dfa(DFA_Interface_load(argv[1]));
	fprintf(stderr, "loaded dfa: %s\n", argv[1]);
	PinYinDFA_Builder pinyin_dfa_builder(argv[2]);
	fprintf(stderr, "built pinyin_dfa_builder: %s\n", argv[2]);
	nark::LineBuf line;
	nark::profiling pf;
	while (line.getline(stdin) > 0) {
		line.chomp();
		printf("input: %s\n", line.p);
		long long t2 = pf.now();
		DFA_InterfacePtr pinyin_dfa(pinyin_dfa_builder.make_pinyin_dfa(line, NULL));
		long long t3 = pf.now();
		if (pinyin_dfa.get() != NULL) {
			PinYinDFA_Builder::match_pinyin(dfa.get(), pinyin_dfa.get(), &OnMatch);
			long long t4 = pf.now();
			printf("time: make_dfa=%f'us match=%f'us\n", pf.uf(t2,t3), pf.uf(t3,t4));
		} else {
			fprintf(stderr, "make_pinyin_dfa failed: %s\n", line.p);
		}
	}
	return 0;
}

