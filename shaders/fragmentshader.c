#version 330 core

in vec2 fragUV;
in vec3 fragNr;
in vec3 fragPos;

out vec4 outColor;

uniform vec2 screenResoution;//屏幕的分辨率
uniform vec2 nearPlane;//相机近切面的三维空间尺寸(左下角放在(0,0,near)的位置,右上角是(nearPlane.x,nearPlane.y,near)的位置)
uniform float near;

//构造一个射线类(传入射线的起点和方向)
struct Ray
{
	vec3 origin;//起点
	vec3 direction;//方向
};
//构造一个球类的父类(可被击中类)
struct Hittable
{
	vec3 center;
	float radius;
};
//一个World类里面存放N多个Hittable
struct World
{
	int hittableCount;
	Hittable hittables[20];//最大容量20个
};

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
	bool hitFront;//是否从正面击中
};

//判断射线ray是否击中圆心为center半径为radius的小球(另外再加入一对最近击中距离和最远击中距离,超出这个范围的默认无碰撞)
HitResult GetHitPos(Hittable object,Ray ray,vec2 scope) {
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
		return hitresult;
	}
	t = (-b + sqrt(discri)) / (2.0 * a);//二次方程求根公式(取较远的点)
	if (t>scope.x && t<scope.y) {
		hitresult.hit = true;
		hitresult.hitT = t;
		hitresult.hitPos = At(ray, t);
		hitresult.hitNr = normalize(hitresult.hitPos - object.center);
		hitresult.hitFront = dot(ray.direction, hitresult.hitNr) < 0;// 射线方向与法线方向点乘判断他们之间的夹角
		return hitresult;
	}
	hitresult.hit = false;
	return hitresult;
}

//根据射线渲染颜色(传进来一个World)
vec3 RayColor(Ray ray) {
	//世界
	World world;
	world.hittableCount = 2;
	world.hittables[0].center = vec3(0.0, 0.0, -3.0);
	world.hittables[0].radius = 0.5;
	world.hittables[1].center = vec3(0.0, -100.5, -3.0);
	world.hittables[1].radius = 100.0;
	//检测碰撞
	vec2 scope = vec2(0.01, 10000.0);//一开始给一个很大的碰撞检测范围
	for (int i = 0; i < world.hittableCount; i++) {
		HitResult hitResult = GetHitPos(world.hittables[i], ray, scope);
		if (hitResult.hit) {
			return hitResult.hitNr * 0.5 + 0.5;//映射到0.0-1.0范围
		}
	}
	float t = ray.direction.y * 0.5 + 0.5;//把方向向量的y轴映射到0.0-1.0之间
	vec3 tempresult = (1.0 - t) * vec3(1.0) + t * vec3(0.5, 0.7, 1.0);
	return tempresult;
}

void main()
{
	vec3 tempvec3 = vec3(fragUV.x * nearPlane.x, fragUV.y * nearPlane.y, -near);//获得了射线方向向量(注意相机朝向Z轴负方向)
	tempvec3.x -= 0.5 * nearPlane.x; tempvec3.y -= 0.5 * nearPlane.y;//注意nearPlane是在大于0的UV空间,所以要移动到原点位置
	Ray ray = GenRay(vec3(0.0), normalize(tempvec3));//c创建一个射线
	vec3 tempresult = RayColor(ray);
	outColor = vec4(tempresult, 1.0);
}