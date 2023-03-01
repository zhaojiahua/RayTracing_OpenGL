#version 330 core
#define PI 3.1415926535898

in vec2 fragUV;
in vec3 fragNr;
in vec3 fragPos;

out vec4 outColor;

uniform vec2 screenResoution;//屏幕的分辨率
uniform vec2 nearPlane;//相机近切面的三维空间尺寸(左下角放在(0,0,near)的位置,右上角是(nearPlane.x,nearPlane.y,near)的位置)
uniform float near;
uniform mat4 viewM;//相机视图矩阵

const int SAMPLECOUNT = 100;
const int RAYLIMIT = 50;
uniform vec2[SAMPLECOUNT] sampleoffsets;//从CPU端传入100个0-1之间的随机数用于对单个像素的随机采样100次,然后求平均,从而达到抗锯齿的效果

//随机数
uint m_u = uint(521288629);
uint m_v = uint(362436069);
uint GetUintCore(inout uint u, inout uint v){
	v = uint(36969) * (v & uint(65535)) + (v >> 16);
	u = uint(18000) * (u & uint(65535)) + (u >> 16);
	return (v << 16) + u;
}
float GetUniformCore(inout uint u, inout uint v){
	uint z = GetUintCore(u, v);
	
	return float(z) / uint(4294967295);
}
float GetUniform(){
	return GetUniformCore(m_u, m_v);
}
uint GetUint(){
	return GetUintCore(m_u, m_v);
}
float rand(){
	return GetUniform();
}
vec2 rand2(){
	return vec2(rand(), rand());
}

//构造一个射线类(传入射线的起点和方向)
struct Ray
{
	vec3 origin;//起点
	vec3 direction;//方向
};
//构造一个材质类(每个材质类都有固有色,漫反射率,金属度,粗糙度等属性)
struct Materail
{
	vec3 diffuse;
	float metallic;
	float roughness;
	float reflectivity;//物质的反射率(它是随着光线入射角度而发生改变的,越是贴近表面反射率越大,这里给定的是垂直射向表面时的反射率,且遵循能量守恒,折射率=1-反射率)
	float refractive;//折射率(这个折射率指的是光线进入不同介质发生偏折的程度,人为规定空气折射率为1)
	float refractive_damping;//这是由金属度和透明度决定的折射衰减率(一般金属会把折射的光线全部吸收掉,而带透明属性的非金属会吸收一部分折射光线,透过一部分光线,或者可以直接叫透明度)
};
uniform Materail[12] materials;//12中材质

//构造一个球类的父类(可被击中类)
struct Hittable
{
	vec3 center;
	float radius;
	//每个物体还应该具有自己的材质属性
	int mat;
};
//一个World类里面存放N多个Hittable
struct World
{
	int hittableCount;
	Hittable hittables[150];//最大容量150个
};
//世界(world里面包含大量的小球,由CPU传入进来)
uniform World world;

//给定射线的起点和方向创建一个射线
Ray GenRay(vec3 org,vec3 dir) {
	Ray r;
	r.origin = org;
	r.direction = dir;
	return r;
}

//给定射线和沿射线方向的距离获取这个射线上的点
vec3 At(Ray ray, float t) {
	return ray.origin + t * ray.direction;
};

//构造一个击中结果的类用来击中函数返回数据包用的(返回是否击中,击中的位置等信息)
struct HitResult
{
	bool hit;//是否击中
	float hitT;//击中点的t偏移量
	vec3 hitPos;//击中的位置
	vec3 hitNr;//击中位置的面法线
	vec3 hitReflectDir;//击中的点的光线的反射方向
	vec3 hitRefractDir;//击中点的光线的折射方向
	bool hitFront;//是否从正面击中
	int hitMat;//击中点的所属材质
};

//计算漫反射的方向
vec3 RandReflectDirection(vec3 inDir,vec3 inN,int inmat){
	vec3 refDir=reflect(normalize(inDir),normalize(inN));//反射方向
	vec2 randvec2=rand2();
	randvec2.x*=(0.5*PI);
	randvec2.y*=(2.0*PI);
	float theta=randvec2.x;
	float phi=randvec2.y;
	vec3 randDir=vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
	return normalize(refDir* materials[inmat].metallic+randDir*(1.0- materials[inmat].metallic)+randDir* materials[inmat].roughness);//随机方向向反射方向收缩(收缩程度决定了高光强度,由不同的材质决定)
}
//计算折射方向
vec3 GetRefractDirection(vec3 inDir,vec3 inN,int inmat){
	float cos_theta=dot(-inDir,inN);
	vec3 r_out_parallel= materials[inmat].refractive * (inDir + cos_theta * inN);
	vec3 r_out_perp=-sqrt(1.0-length(r_out_parallel)*length(r_out_parallel))*inN;
	return r_out_parallel+r_out_perp;//折射方向
};
vec3 GetRefractDirectionBF(vec3 inDir,vec3 inN,int inmat){
	vec3 v=normalize(inDir);
	vec3 n=normalize(inN);
	float dt=dot(v,n);
	float discriminant=1.0- materials[inmat].refractive* materials[inmat].refractive*(1.0-dt*dt);
	if(discriminant>0.0){
		return materials[inmat].refractive*(v-n*dt)-n*sqrt(discriminant);
	}
	else return vec3(0.0);
};

//判断射线ray是否击中圆心为center半径为radius的小球(另外再加入一对最近击中距离和最远击中距离,超出这个范围的默认无碰撞)
HitResult GetHitPos(Hittable object,Ray ray,vec2 scope) {
	object.center = (viewM * vec4(object.center, 1.0)).rgb;//为了配合相机的变动,这里需要对object的位置进行视图矩阵变换
	HitResult hitresult;
	vec3 a_c = ray.origin - object.center;
	float a = dot(ray.direction, ray.direction);
	float b = 2 * dot(a_c, ray.direction);
	float c = dot(a_c, a_c) - object.radius * object.radius;
	float discri = b * b - 4 * a * c;
	if (discri < 0) {
		hitresult.hit = false;//二次方程根判别式小于0说明没有交点
		return hitresult;
	}
	float t = (-b - sqrt(discri)) / (2.0 * a);//二次方程求根公式(取较近的点)
	if (t>scope.x && t<scope.y) {
		hitresult.hit = true;
		hitresult.hitT = t;
		hitresult.hitPos = At(ray, t);
		hitresult.hitNr = normalize(hitresult.hitPos - object.center);
		hitresult.hitFront = dot(ray.direction, hitresult.hitNr) < 0;// 射线方向与法线方向点乘判断他们之间的夹角
		hitresult.hitReflectDir=RandReflectDirection(ray.direction,hitresult.hitNr,object.mat);//随机散射的方向
		hitresult.hitRefractDir=GetRefractDirection(ray.direction,hitresult.hitNr,object.mat);
		if(hitresult.hitFront) hitresult.hitMat=object.mat;
		return hitresult;
	}
	t = (-b + sqrt(discri)) / (2.0 * a);//二次方程求根公式(取较远的点)
	if (t>scope.x && t<scope.y) {
		hitresult.hit = true;
		hitresult.hitT = t;
		hitresult.hitPos = At(ray, t);
		hitresult.hitNr = normalize(hitresult.hitPos - object.center);
		hitresult.hitFront = dot(ray.direction, hitresult.hitNr) < 0;// 射线方向与法线方向点乘判断他们之间的夹角
		hitresult.hitReflectDir=RandReflectDirection(ray.direction,hitresult.hitNr,object.mat);
		hitresult.hitRefractDir=GetRefractDirection(ray.direction,hitresult.hitNr,object.mat);
		if(hitresult.hitFront)	hitresult.hitMat=object.mat;
		return hitresult;
	}
	hitresult.hit = false;
	return hitresult;
}

//根据射线渲染颜色(传进来一个射线和反弹次数限制)
vec3 RayColor(Ray ray,int limit) {

	//环境光(没有碰撞到任何物体时候的颜色)
	float t = ray.direction.y * 0.5 + 0.5;//把方向向量的y轴映射到0.0-1.0之间
	vec3 tempresult = (1.0 - t) * vec3(1.0) + t * vec3(0.5, 0.7, 1.0);//用来存储漫反射结果(其初始颜色是环境光)
	
	//反射方向跟踪
	HitResult hitResult;
	vec3 tempresult_reflect=tempresult;
	vec3 tempresult_refract=tempresult;
	Ray ray_reflect=ray;
	Ray ray_refract=ray;
	float refractInc=1.0;
	//反射次数限制内反射检测
	while (limit > 0)
	{
		limit--;
		vec2 scope = vec2(0.001, 10000.0);//一开始给一个很大的碰撞检测范围(每次循环要更新)
		bool hitAny=false;
		for (int i = 0; i < world.hittableCount; i++) {
			hitResult = GetHitPos(world.hittables[i], ray, scope);//注意这里在极短的时间里可能会与自己发生碰撞,所以scope一定要给值
			if(hitResult.hit && hitResult.hitFront){
				hitAny=true;
				scope.y=hitResult.hitT;//获得所有物体最近的击中点
			}
		}
		if(!hitAny) break;//如果遍历了所有的物体后都没有击中物体就跳出循环
		ray.origin = hitResult.hitPos;
		//一定概率的发生反射或折射(发生反射的概率正好是物质的反射率)
		if(rand()>materials[hitResult.hitMat].reflectivity && materials[hitResult.hitMat].metallic<0.01){//只要稍微有点金属度我们就不给它折射的机会
			tempresult = (refractInc* materials[hitResult.hitMat].diffuse);
			refractInc*=(1.0- materials[hitResult.hitMat].reflectivity);
			ray.direction = hitResult.hitRefractDir;
		}
		else{
			tempresult*= materials[hitResult.hitMat].diffuse;
			ray.direction = hitResult.hitReflectDir;
		}
	}
	return tempresult;
}

void main()
{
	vec3 tempvec3 = vec3(fragUV.x * nearPlane.x, fragUV.y * nearPlane.y, -near);//获得了射线方向向量(注意相机朝向Z轴负方向)
	tempvec3.x -= 0.5 * nearPlane.x; tempvec3.y -= 0.5 * nearPlane.y;//注意nearPlane是在大于0的UV空间,所以要移动到原点位置
	float dx = nearPlane.x / float(screenResoution.x);
	float dy = nearPlane.y / float(screenResoution.y);
	//生成100条射线然后执行100次RayColor,最后求平均值
	vec3 tempresult = vec3(0.0f);
	for (int i = 0; i < SAMPLECOUNT; i++) {
		vec3 temptempvec3 = tempvec3;
		temptempvec3.x += sampleoffsets[i].x * dx; temptempvec3.y += sampleoffsets[i].y * dy;
		Ray ray = GenRay(vec3(0.0), normalize(temptempvec3));//创建一个射线
		tempresult += RayColor(ray, RAYLIMIT);//限制50次反弹
	}
	tempresult *= 0.01;
	tempresult=vec3(pow(tempresult.x,1.0/2.2),pow(tempresult.y,1.0/2.2),pow(tempresult.z,1.0/2.2));//gamma校正
	outColor = vec4(tempresult,1.0);
}