//main.c
char myArray[12] = { 'm', 'e', 'c', 'c', 'a', 't', 'r', 'o', 'n', 'i', 'c',
			'o' };

	struct myStruct example_struct;
	int j = 0;
	char c;

	for (int k = 0; k < 12; k++) {

		for ( c = 'a'; c <= 'z'; c++) {

			if (c == myArray[k]) {

				example_struct.alfChar[j] = c;
				example_struct.alfInt[j] = example_struct.alfInt[j] + 1;
				break;


			} else {
				example_struct.alfChar[j] = 'X';
				example_struct.alfInt[j] = 0;

			}



		}
		j++;

	}

//myStruc.h

struct myStruct {
	  char alfChar[26];
	  uint8_t alfInt[26];
  };
