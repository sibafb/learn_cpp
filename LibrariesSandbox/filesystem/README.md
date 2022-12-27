## filesystem library 

```
./filesystem_pick_extension.cpp
```


### filesystem_pick_extension.cpp

- Target Directory

```
filesystem$ tree samplefiles/
samplefiles/
├── bar.tar.gz
├── foo
│   └── bar.json
├── foobar
├── foo_bar.txt
└── foo.json
```

- Execution result

```
filesystem/build$./filesystem 
directory_iterator_result
"../samplefiles/foo.json"
recursive_directory_iterator_result
"../samplefiles/foo/bar.json"
"../samplefiles/foo.json"
```