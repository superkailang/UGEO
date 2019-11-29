## Source Code Analysis
#####

All souce codes require a C compiler

UGEO project is well organized, include three main aspects

* File IO
* Grid Process
* Grid Visilazation

### Architecture

![UGEO](https://vueproject-1253784566.cos.ap-chengdu.myqcloud.com/github/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20191129205306.png)


### [Base Struct](https://github.com/superkailang/UGEO/blob/master/src/UGEO/Gridstruct.h)
In gridStruct file, Grid type and are defined

```c++
	struct CPCell
	{
		CPCell() :is_act(true) {};
		point corner[8]; // corner points
		point facec[6];
		point blockc;
		bool is_act;
		point AreaVec(const int dir);
		double Gen_GeomCenter(); // return volume
	};
```

File IO
