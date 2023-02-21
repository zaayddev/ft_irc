
#include <iostream>
#include <vector>

struct Table{
    int     index;
    char    character;
    int     value;
};

int romanToInt(std::string s) {
      int i = 0;
      int returnValue = 0;
      std::vector<Table> table;
      Table tab1 = {1, 'I', 1};
      Table tab2 = {2, 'V', 5};
      Table tab3 = {3, 'X', 10};
      Table tab4 = {4, 'L', 50};
      Table tab5 = {5, 'C', 100};
      Table tab6 = {6, 'D', 500};
      Table tab7 = {7, 'M', 1000};
      table.__emplace_back(tab1);
      table.__emplace_back(tab2);
      table.__emplace_back(tab3);
      table.__emplace_back(tab4);
      table.__emplace_back(tab5);
      table.__emplace_back(tab6);
      table.__emplace_back(tab7);
      while (s[i]) {
         std::vector<Table>::iterator it = table.begin();
		 while (it != table.end()){
            Table tab = *it;
            if (tab.character == s[i]) {
				for (auto bat:table) {
					if (bat.character == s[i+1]) {
                		if (tab.index < bat.index){
                    		returnValue += (bat.value - tab.value);
                    		i++;
							it++;
							std::cerr << bat.value - tab.value << std::endl;
                		} else {
							returnValue += tab.value;
						}
					}
				}
               	 returnValue += tab.value;
            }
			it++;
          }
		  i++;
      }  
      return returnValue;
    }

int main () {
	std::string s="MCMXCIV";
	int i = romanToInt(s);
	std::cout << i << std::endl;
}
