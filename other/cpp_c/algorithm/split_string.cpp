#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
using namespace std;

class string_split
{
public:
    string_split(string str_src = string(""))
    {
        this->str_src = str_src;
        i_pos = 0;
    }

    void split(vector<string>& vec_dst_str, string& seperator, bool b_combine)
    {
        vec_dst_str.clear();

        while(i_pos != str_src.npos)
        {
            string str_substring = get_next_substring(seperator, b_combine);

            if (str_substring.length())
            {
                vec_dst_str.push_back(str_substring);
            }
        }
    }

private:
    string str_src;
    int i_pos;

    int find_next_seperator(string& seperator, int i_start_pos, bool b_combine)
    {
        int i_end_pos = str_src.npos;

        if (b_combine)
        {
            i_end_pos = str_src.find(seperator, i_start_pos);
        }
        else
        {
            int i_sep_len = seperator.length();

            for(int i = 0; i < i_sep_len; i++)
            {
                char c_sep = seperator.at(i);
                int i_sep_pos = str_src.find(c_sep, i_start_pos);

                if (i_sep_pos != str_src.npos)
                {
                    bool b_need_pass = i_end_pos != str_src.npos && i_end_pos <= i_sep_pos;

                    if (!b_need_pass)
                    {
                        i_end_pos = i_sep_pos;
                    }
                }
            }
        }

        return i_end_pos;
    }

    string get_next_substring(string& seperator, bool b_combine)
    {
        int i_start_pos = i_pos;
        int i_end_pos = find_next_seperator(seperator, i_start_pos, b_combine);
		int i_string_length = 0;

        if (i_end_pos != str_src.npos)
        {
        	if(b_combine)
        	{
				i_pos = i_end_pos + seperator.length();
			}
			else
			{
				i_pos = i_end_pos + 1;
			}
            i_string_length = i_end_pos - i_start_pos ;
        }
        else
        {
        	i_pos = i_end_pos;
            i_string_length = i_end_pos;
        }

        return str_src.substr(i_start_pos, i_string_length);
    }
};




void my_split_string( vector<string>& vec_string, string& src_string, char *cp_delimiter, bool b_combination )
{
    size_t i_str_length = src_string.length();

    if(!i_str_length && !cp_delimiter)
    {
        return;
    }

    vec_string.clear();

    char *cp = new char[i_str_length + 1];

    if(cp)
    {
        memset(cp, 0, i_str_length + 1);

        memcpy(cp, src_string.c_str(), i_str_length);

        for(unsigned int i = 0; i <= i_str_length; i++)
        {
            char *cp_delimiter_tmp = cp_delimiter;

            if(b_combination)
            {
                size_t i_delimiter_len = strlen(cp_delimiter_tmp);

                if (i + i_delimiter_len > i_str_length + 1)
                {
                    break;
                }

                bool b_match = true;
                size_t i_offset = 0;

                while(i_offset < i_delimiter_len)
                {
                    if(i + i_offset >= i_str_length)
                    {
                        b_match = false;
                        break;
                    }

                    if(cp[i + i_offset] != cp_delimiter_tmp[i_offset])
                    {
                        b_match = false;
                        break;
                    }
                    else
                    {
                        i_offset++;
                    }
                }

                if(b_match)
                {
                    memset(&cp[i], 0, i_delimiter_len);
                    i += i_delimiter_len - 1;
                }
            }
            else
            {
                while(*cp_delimiter_tmp)
                {
                    if(cp[i] == *cp_delimiter_tmp)
                    {
                        cp[i] = '\0';
                        break;
                    }
                    else
                    {
                        cp_delimiter_tmp++;
                    }
                }
            }
        }

        char *cpStr = cp;

        while(cpStr < cp + i_str_length)
        {
            if(*cpStr != '\0')
            {
                size_t iLen = strlen(cpStr);
                string str_str = cpStr;

                //MessageBox(0,str_str.c_str(), "split", MB_OK);
                vec_string.push_back(str_str);
                cpStr += iLen;
            }
            else
            {
                cpStr++;
            }
        }

        delete cp;
        cp = NULL;
    }
}


int main()
{

    string s = "xiofeiajdlfjsaljdflsa<sep>sdjfls<sep>sdfjlkasjdfl";
    char *cp_sep = "<sep>";
    vector<string> vec_dst_string;

    my_split_string(vec_dst_string, s, cp_sep, true);

    vector<string>::iterator it = vec_dst_string.begin();
    for(; it != vec_dst_string.end(); it++)
    {
        cout << it->c_str() << endl;
    }


	string_split s_s("xiofeiajdlfjsaljdflsa<sep>sdjfls<sep>sdfjlkasjdfl");

	string s_sep = cp_sep;
	s_s.split(vec_dst_string, s_sep, true);
    it = vec_dst_string.begin();
    for(; it != vec_dst_string.end(); it++)
    {
        cout << it->c_str() << endl;
    }

    return 0;
}