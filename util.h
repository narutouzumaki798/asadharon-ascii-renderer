int digits(int x)
{
	if(x == 0) return 1;
	int ans = 0;
	while(x) { x=x/10; ans++; }
	return ans;
}
string to_string1(int x)
{
	if(x == 0) return "0";
	string ans;
	while(x)
	{
		ans += ('0' + (x%10));
		x = x/10;
	}
	reverse(ans.begin(), ans.end());
	return ans;
}
void show_util(double x)
{
	int flag = 0;
	if(x < 0) { x = -x; flag = 1; }

	int a = (int)(x*1000);
	int b = a%1000;
	a = a/1000;            // a = before [.] , b = after [.]


	string p,q; int tmp;
	p = to_string1(a); q = to_string1(b);

	if(flag) p = "-" + p;
	tmp = 4 - p.size();
	while(tmp > 0) { p = " " + p; tmp--; }

	tmp = 4 - q.size();
	while(tmp > 0) { q = q + "0"; tmp--; }

	if(flag)
	printf("%s.%s ", p.c_str(), q.c_str());
	else
	printf("%s.%s ", p.c_str(), q.c_str());
}

