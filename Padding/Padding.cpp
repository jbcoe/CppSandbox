struct Small
{
	short s;
	int i;
	double d;
	long l;
};

static_assert(sizeof(bool)==1,"bool");
static_assert(sizeof(short)==2,"short");
static_assert(sizeof(int)==4,"int");
static_assert(sizeof(double)==8,"double");
static_assert(sizeof(long)==8,"long");

static_assert(sizeof(Small)==24,"24");

static_assert(sizeof(Small)==(
			sizeof(short)+2+
			sizeof(int)+
			sizeof(double)+
			sizeof(long)
			),"Small is not the size one would expect");

struct Large
{
	short s;
	double d;
	long l;
	int i;
};

static_assert(sizeof(Large)==(2+6+8+8+4+4),
		"Large is not the size one would expect");

struct Unpadded
{
	short s1;
	short s2;
	short s3;
	short s4;
};

struct HighlyUnpadded
{
	short s1;
	bool b1;
	bool b2;
	short s3;
	short s4;
};

static_assert( sizeof(Large) > sizeof(Small), "Large should be bigger than Small");

