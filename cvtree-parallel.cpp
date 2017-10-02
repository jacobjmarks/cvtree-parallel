#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <fstream>

using namespace std;

int number_bacteria;
vector<string> bacteria_name;
long M, M1, M2;
short code[27] = { 0, 2, 1, 2, 3, 4, 5, 6, 7, -1, 8, 9, 10, 11, -1, 12, 13, 14, 15, 16, 1, 17, 18, 5, 19, 3};
#define encode(ch)		code[ch-'A']
#define LEN				6
#define AA_NUMBER		20
#define	EPSILON			1e-010

void Init() {
	M2 = 1;
	for (int i = 0; i < LEN-2; i++) {	// M2 = AA_NUMBER ^ (LEN-2);
		M2 *= AA_NUMBER; 
	}
	M1 = M2 * AA_NUMBER;		// M1 = AA_NUMBER ^ (LEN-1);
	M  = M1 *AA_NUMBER;			// M  = AA_NUMBER ^ (LEN);
}

class Bacteria {
private:
	vector<long> vectorA;
	vector<long> vectorB;
	long one_l[AA_NUMBER];
	long indexs;
	long total;
	long total_l;
	long complement;

	void InitVectors() {
		vectorA = vector<long>(M, 0.0);
		vectorB = vector<long>(M1, 0.0);
		memset(one_l, 0, AA_NUMBER * sizeof(long));
		total = 0;
		total_l = 0;
		complement = 0;
	}

	void init_buffer(char* buffer) {
		complement++;
		indexs = 0;
		for (int i = 0; i < LEN-1; i++) {
			short enc = encode(buffer[i]);
			one_l[enc]++;
			total_l++;
			indexs = indexs * AA_NUMBER + enc;
		}
		vectorB[indexs]++;
	}

	void cont_buffer(char ch) {
		short enc = encode(ch);
		one_l[enc]++;
		total_l++;
		long index = indexs * AA_NUMBER + enc;
		vectorA[index]++;
		total++;
		indexs = (indexs % M2) * AA_NUMBER + enc;
		vectorB[indexs]++;
	}

public:
	long count;
	double* tv;
	long *ti;

	Bacteria(const char* filename) {
		#pragma omp critical
		{
			FILE * bacteria_file = fopen(filename,"r");
			if (bacteria_file == NULL) {
				throw invalid_argument("BAD DATA DIRECTORY. Please specify full relative path.");
			}
			InitVectors();

			char ch;
			while ((ch = fgetc(bacteria_file)) != EOF) {
				if (ch == '>') {
					while (fgetc(bacteria_file) != '\n'); // skip rest of line

					char buffer[LEN-1];
					fread(buffer, sizeof(char), LEN-1, bacteria_file);
					init_buffer(buffer);
				} else if (ch == '\r') {
					// Skip carriage return.
				} else if (ch != '\n') {
					cont_buffer(ch);
				}
			}
			fclose (bacteria_file);
		}

		long total_plus_complement = total + complement;
		double total_div_2 = total * 0.5;
		int i_mod_aa_number = 0;
		int i_div_aa_number = 0;
		long i_mod_M1 = 0;
		long i_div_M1 = 0;

		double one_l_div_total[AA_NUMBER];
		for (int i = 0; i < AA_NUMBER; i++) {
			one_l_div_total[i] = (double)one_l[i] / total_l;
		}
		
		double* second_div_total = new double[M1];
		for (int i = 0; i < M1; i++) {
			second_div_total[i] = (double)vectorB[i] / total_plus_complement;
		}

		count = 0;
		double* t = new double[M];

		for(long i = 0; i < M; i++) {
			double p1 = second_div_total[i_div_aa_number];
			double p2 = one_l_div_total[i_mod_aa_number];
			double p3 = second_div_total[i_mod_M1];
			double p4 = one_l_div_total[i_div_M1];
			double stochastic =  (p1 * p2 + p3 * p4) * total_div_2;

			if (i_mod_aa_number == AA_NUMBER-1) {
				i_mod_aa_number = 0;
				i_div_aa_number++;
			} else {
				i_mod_aa_number++;
			}

			if (i_mod_M1 == M1-1) {
				i_mod_M1 = 0;
				i_div_M1++;
			} else {
				i_mod_M1++;
			}

			if (stochastic > EPSILON) {
				t[i] = (vectorA[i] - stochastic) / stochastic;
				count++;
			} else {
				t[i] = 0;
			}
		}
		
		delete second_div_total;
		vector<long>().swap(vectorA);
		vector<long>().swap(vectorB);

		tv = new double[count];
		ti = new long[count];

		int pos = 0;
		for (long i = 0; i < M; i++) {
			if (t[i] != 0) {
				tv[pos] = t[i];
				ti[pos] = i;
				pos++;
			}
		}
		delete t;
	}
};

void ReadDataDir(char* data_dir) {
	// Retrieve filenames in data directory.
	// Based on https://stackoverflow.com/a/612176
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (data_dir)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			string filename = ent->d_name;
			if (filename != "." && filename != "..") {
				bacteria_name.push_back(data_dir + filename);
				number_bacteria++;
			}
		}
		closedir (dir);
		sort(bacteria_name.begin(), bacteria_name.end());
	} else {
		perror ("");
	}
}

double CompareBacteria(Bacteria* b1, Bacteria* b2) {
	double correlation = 0;
	double vector_len1=0;
	double vector_len2=0;
	long p1 = 0;
	long p2 = 0;

	while (p1 < b1->count && p2 < b2->count) {
		long n1 = b1->ti[p1];
		long n2 = b2->ti[p2];

		if (n1 < n2) {
			double t1 = b1->tv[p1];
			vector_len1 += (t1 * t1);
			p1++;
		} else if (n2 < n1) {
			double t2 = b2->tv[p2];
			p2++;
			vector_len2 += (t2 * t2);
		} else {
			double t1 = b1->tv[p1++];
			double t2 = b2->tv[p2++];
			vector_len1 += (t1 * t1);
			vector_len2 += (t2 * t2);
			correlation += t1 * t2;
		}
	}

	while (p1 < b1->count) {
		long n1 = b1->ti[p1];
		double t1 = b1->tv[p1++];
		vector_len1 += (t1 * t1);
	}

	while (p2 < b2->count) {
		long n2 = b2->ti[p2];
		double t2 = b2->tv[p2++];
		vector_len2 += (t2 * t2);
	}

	return correlation / (sqrt(vector_len1) * sqrt(vector_len2));
}

void CompareAllBacteria() {
	Bacteria** b = new Bacteria*[number_bacteria];
	
	#pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < number_bacteria; i++) {
		printf("[%d] load %.2d of %.2d - %s\n", omp_get_thread_num()+1, i+1, number_bacteria, bacteria_name[i].c_str());
		b[i] = new Bacteria(bacteria_name[i].c_str());
	}

	vector<vector<double>> results;

	printf("Comparing...");
	#pragma omp parallel for ordered schedule(dynamic)
    for(int i = 0; i < number_bacteria-1; i++) {
		#pragma omp critical
		{
		results.push_back(vector<double>(number_bacteria, 0.0));			
		}
		for(int j = i+1; j < number_bacteria; j++) {
			double correlation = CompareBacteria(b[i], b[j]);
			#pragma omp critical
			{
			results[i][j] = correlation;
			}
			// printf("[%d] %.2d %.2d -> %.20lf\n", omp_get_thread_num()+1, i, j, correlation);
		}
	}
	printf("Done!\n");
	
	printf("Writing results...");
	ofstream output;
	output.open("results-parallel.csv");
	output << "indexA" << ',' << "bacteriaA" << ',' << "indexB" << ',' << "bacteriaB" << ',' << "correlation" << endl;	
	for (int i = 0; i < number_bacteria-1; i++) {
		for (int j = i+1; j < number_bacteria; j++) {
			output << i << ',' << bacteria_name[i] << ',' << j << ',' << bacteria_name[j] << ',' << results[i][j] << endl;
		}
	}	
	output.close();
	printf("Done!\n");
}

int main(int argc,char * argv[]) {
	time_t t1 = time(NULL);

	Init();
	ReadDataDir(argv[1]);
	CompareAllBacteria();

	time_t t2 = time(NULL);
	printf("time elapsed: %d seconds\n", t2 - t1); 
	return 0;
}