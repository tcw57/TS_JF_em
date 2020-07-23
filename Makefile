all: jet_finding

jet_finding: int_em_v5.cpp L1_cluster_v5.cpp L2_cluster_v5.cpp
	g++ -g -Werror -Wall int_em_v5.cpp L1_cluster_v5.cpp L2_cluster_v5.cpp -o jet_find

compare: comparesim_L2v4.cpp
	g++ -g -Werror -Wall comparesim_L2v4.cpp -o compare

clean: all
	rm -rf jet_find
