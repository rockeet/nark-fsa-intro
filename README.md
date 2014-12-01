nark-fsa-intro
==============

This project is just the introduction of `nark-fsa`, `nark-fsa` is private on github.

`nark-fsa` is a Finite State Automata library which implemented:
* [High compression dictionary with hierachical keys](#adfa)
* [DAWG(Directed Acyclic Word Graph)](#dawg)
* [Multiple Regular Expression matching](#regex)
* [Aho-Corasick automata](#ahoc)
* [Hocroft DFA minimization algorithm](#hopcroft)
* [Offline Linear time Acyclic DFA minimization algorithm](#offline-adfa)
* [Online Linear time Acyclic DFA minimization algorithm](#online-adfa)
* [Load DFA file through Memory Mapping](#dfa-mmap)

<h2 id="adfa">High compression dictionary with hierachical keys</h2>
### Features
  1. Fast
  1. Small: much smaller than input data set (usually 3+ times smaller)
  1. prefix search: search keys which is a prefix of input text
  1. predictive search: search suffixies which prefixies matche input text
  1. value search: value search is a kind of predictive search

<h2 id="dawg">DAWG(Directed Acyclic Word Graph)</h2>

To be done ...

<h2 id="regex">Multiple Regular Expression matching</h2>
To be done ...

<h2 id="ahoc">Aho-Corasick automata</h2>
There are two implementation of Aho-Corasick automata: Compact Dynamic DFA based and double array trie based.

To be done ...

<h2 id="hopcroft">Hocroft DFA minimization algorithm</h2>
To be done ...

<h2 id="offline-adfa">Offline Linear time Acyclic DFA minimization algorithm</h2>
To be done ...

<h2 id="online-adfa">Online Linear time Acyclic DFA minimization algorithm</h2>
To be done ...

<h2 id="dfa-mmap">Load DFA file through Memory Mapping</h2>
To be done ...


