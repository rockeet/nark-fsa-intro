#define _SCL_SECURE_NO_WARNINGS
#include <nark/fsa/dfa_interface.hpp>
#include <getopt.h>

using namespace nark;

int main(int argc, char* argv[]) {
	const char* ifile = NULL;
	for (int opt=0; (opt = getopt(argc, argv, "i:")) != -1; ) {
		switch (opt) {
			case '?': return 3;
			case 'i': ifile = optarg; break;
		}
	}
	std::auto_ptr<DFA_Interface> dfa;
	if (ifile) dfa.reset(DFA_Interface::load_from(ifile)); // by filename
	else       dfa.reset(DFA_Interface::load_from(stdin)); // by FILE*
	const DAWG_Interface* dawg = dfa->get_dawg();
	if (NULL == dawg) {
		fprintf(stderr, "file \"%s\" is not a dawg\n", ifile?ifile:"stdin");
		return 1;
	}
	size_t num_words = dawg->num_words();
	for (int i = optind; i < argc; ++i) {
		const char* szidx = argv[i];
		size_t idx = strtoul(szidx, NULL, 10);
		if (idx >= dawg->num_words()) {
			fprintf(stderr, "idx=%zd >= num_words=%zd\n", idx, num_words);
		} else {
			// dawg->nth_word(idx) will throw std::out_of_range for larger idx
			std::string word = dawg->nth_word(idx);
			printf("%08zd  %s\n", idx, word.c_str());
		}
	}
	return 0;
}

