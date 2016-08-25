#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "curl/curl.h"

#include "curl/easy.h"
#include "json/json.h"
#define MAX_BUFFER_SIZE 512
#define MAX_BODY_SIZE 1000000
using namespace std;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
static inline bool is_base64(unsigned char c) 
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) 
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) 
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) 
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';
	}
	return ret;
}

string base64_decode(std::string const& encoded_string) 
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) 
	{
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) 
		{
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j <4; j++)
			char_array_4[j] = 0;


		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	return ret;
}

//回调函数
static size_t writefunc(void *ptr, size_t size, size_t nmemb, char **result)
{
	size_t result_len = size * nmemb;
	*result = (char *)realloc(*result, result_len + 1);
	if (*result == NULL)
	{
		printf("realloc failure!\n");
		return 1;
	}
	memcpy(*result, ptr, result_len);
	(*result)[result_len] = '\0';
	cout<<"百度服务器返回的json数据："<<*result<<endl;
	/*Json::Reader reader;
	Json::Value root;
	if(reader.parse(result,root))
	{
		string res = root["result"].asString();
		cout <<"解析的结果: "<< res << endl;
	}*/
	return result_len;
}

int main()
{
	freopen("out.txt", "w", stdout);
	int json_file_size;
	FILE *pFile = NULL;
	char *audio_data;
	pFile = fopen("test.pcm", "r");
	if (pFile == NULL)
	{
		perror("Open file error!\n");
	}
	else
	{
		fseek(pFile, 0, SEEK_END);
		int file_size = ftell(pFile);
		cout << "file size: " << file_size << " bytes" << endl;
		fseek(pFile, 0, SEEK_SET);
		audio_data = (char *)malloc(sizeof(char)*file_size);
		fread(audio_data, file_size, sizeof(char), pFile);

		//机器的mac地址
		char *cuid = "56:84:7a:fe:97:99";
		char *api_key = "YGyI3APWnLGSiIFUWEnhBEoC";
		char *secret_key = "fc4d81ef45486b17da48d6d2e81b65d5";

		char host[MAX_BUFFER_SIZE];
		snprintf(host, sizeof(host),
			"https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=%s&client_secret=%s",
			api_key, secret_key);
		cout << "curl -s命令的host: " << host << endl;

		FILE *p = NULL;
		char cmd[MAX_BUFFER_SIZE];
		//curl -s命令的返回结果
		char *result = (char*)malloc(MAX_BUFFER_SIZE);
		char *curl_cmd = "curl -s ";
		char *yinhao = "\"";

		strcpy(cmd, curl_cmd);
		strcat(cmd, yinhao);
		strcat(cmd, host);
		strcat(cmd, yinhao);


		p = popen(cmd, "r");
		fgets(result, MAX_BUFFER_SIZE, p);
		cout << "curl -s 响应结果: " << result << endl;
		pclose(p);

		string access_token;
		//解析服务器返回的Json数据,获取access_token
		if (result != NULL)
		{
			cout << "result is not null! come here!" << endl;
			Json::Reader reader;
			Json::Value root;
			if (!reader.parse(result, root, false))
			{
				//access_token = root.get("access_token", "").asString();
				access_token = root["access_token"].asString();
				cout << "access_token: " << access_token << endl;
			}
			//cout << "access_token: " << access_token << endl;
		}

		//采取隐式发送的方式给服务器发送json格式的数据
		char body[MAX_BODY_SIZE];
		memset(body, 0, sizeof(body));
		string decode_data = base64_encode((const unsigned char *)audio_data, file_size);
		if (0 == decode_data.length())
		{
			cout << "Error!base64 encoded data is empty!";
			return 1;
		}
		else
		{
			Json::Value buffer;
			Json::FastWriter buf_writer;
			buffer["format"] = "pcm";
			buffer["rate"] = 8000;
			buffer["channel"] = 1;
			buffer["token"] = access_token.c_str();
			buffer["cuid"] = cuid;
			buffer["speech"] = decode_data;
			buffer["len"] = file_size;
			//实际json格式数据的长度
			json_file_size = buf_writer.write(buffer).length();
			cout << "Json file size:" << json_file_size << " bytes" << endl;
			memcpy(body, buf_writer.write(buffer).c_str(), json_file_size);
			//cout << "Json Body:" << body <<endl;

			CURL *curl;
			CURLcode res;//服务器的响应结果
			char *result_buffer = NULL;
			struct curl_slist *http_header = NULL;
			char temp[MAX_BUFFER_SIZE];
			memset(temp, 0, sizeof(temp));
			snprintf(temp, sizeof(temp), "%s", "Content-Type: application/json; charset=utf-8");
			cout << "temp content-type:" << temp << endl;
			http_header = curl_slist_append(http_header, temp);
			snprintf(temp, sizeof(temp), "Content-Length: %d", json_file_size);
			cout << "temp content-len:" << temp << endl;
			http_header = curl_slist_append(http_header, temp);
			cout << "http-header-temp " << temp <<endl;


			memset(host, 0, sizeof(host));
			snprintf(host, sizeof(host), "%s", "http://vop.baidu.com/server_api");
			cout << "server host: " << host << endl;
			curl = curl_easy_init();
			curl_easy_setopt(curl, CURLOPT_URL, host);//设置访问的URL
			curl_easy_setopt(curl, CURLOPT_POST, 1);//1表示常规的http post请求
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);//设置延时
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_file_size);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);//设置回调函数
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result_buffer);
			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
			{
				printf("perform curl error:%d.\n", res);
				return 1;
			}
			curl_slist_free_all(http_header);
			curl_easy_cleanup(curl);
			
			free(audio_data);
		}
	}

	fclose(pFile);
	return 0;

}
