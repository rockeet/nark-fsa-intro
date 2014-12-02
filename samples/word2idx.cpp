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
	for (int i = optind; i < argc; ++i) {
		const char* word = argv[i];
		size_t idx = dawg->index(word);
		if (size_t(-1) == idx)
			printf("NOT FOUND %s\n", word);
		else
			printf("%08zd  %s\n", idx, word);
	}
	return 0;
}

