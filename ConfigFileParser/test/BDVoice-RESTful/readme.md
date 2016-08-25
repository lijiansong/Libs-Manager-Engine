BDVoice-RESTful Test
===
This archive is the source code regarding using curl&json libs to access Baidu's *Voice Recognition* service, implemented by cpp.

##Compile
You can type the following command to compile it.

```
$ g++ -o bdvoice bdvr.cpp -L . -l json_linux-gcc-4.8_libmt -l curl
```

##Third-Party Libs

[curl](https://curl.haxx.se/) & [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
