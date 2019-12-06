# little-ray-tracer
 一个软件光线追踪器。
![图片](https://github.com/137900114/little-ray-tracer/blob/master/Main/RayTracer1.png)
 虽然简单，这东西在光照复杂的场景里渲染效果爽的一匹（三个可以反射光线的物体啊）：
![图片](https://github.com/137900114/little-ray-tracer/blob/master/Main/RayTracer2.png)
没有优化，没有并发运行起来有点慢，看看以后能不能优化一下

和数学有关的功能全在geometry.h和geometry.cpp里面。
Ray 定义了追踪的光线信息，Insection定义了光线相交点的信息。
每个可以渲染的物体都继承自Shape类实现了insection方法。光线与物体交互时调用insection方法。
