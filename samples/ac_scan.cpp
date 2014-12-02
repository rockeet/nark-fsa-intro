#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <nark/fsa/dfa_interface.hpp>
#include <nark/util/autoclose.hpp>
#include <nark/util/linebuf.hpp>
#include <getopt.h>
#include <malloc.h>

using namespace nark;

struct OnHit {
	void operator()(size_t endpos, const uint32_t* words, size_t cnt) const {
		for (size_t i = 0; i < cnt; ++i) {
			int wlen = ac->wlen(words[i]);
			size_t pos = endpos - wlen;
			printf("hit_endpos=%06zd : word_id=%06d : %.*s\n", endpos, words[i], wlen, text+pos);
		}
	}
	const AC_Scan_Interface* ac;
	const char* text;
};
int main(int argc, char* argv[]) {
	const char* dfa_file = NULL;
	const char* txt_file = NULL;
	for (int opt=0; (opt = getopt(argc, argv, "i:f:")) != -1; ) {
		switch (opt) {
			case '?': return 3;
			case 'i': dfa_file = optarg; break;
			case 'f': txt_file = optarg; break;
		}
	}
	if (NULL == dfa_file) {
		fprintf(stderr, "usage: %s -i input_ac_dfa_file [-t text_file_to_be_matched]\n", argv[0]);
		return 1;
	}
	std::auto_ptr<DFA_Interface> dfa(DFA_Interface::load_from(dfa_file));
	if (dfa->get_ac() == NULL) {
		fprintf(stderr, "Fail: file: %s is not a AC DFA\n", dfa_file);
		return 1;
	}
	nark::Auto_fclose fp;
	if (txt_file) {
		fp = fopen(txt_file, "r");
		if (NULL == fp) {
			fprintf(stderr, "Fail: fopen(%s, r) = %m\n", txt_file);
			return 1;
		}
	}
	OnHit on_hit = { dfa->get_ac(), NULL };
	nark::LineBuf line;
	while (line.getline(fp.self_or(stdin)) > 0) {
		line.chomp();
		on_hit.text = line.p;
		on_hit.ac->ac_scan(line, boost::ref(on_hit));
	}
	return 0;
}

