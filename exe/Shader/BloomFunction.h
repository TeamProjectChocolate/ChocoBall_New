
float g_luminance;	// ‹P“x‚ÌŒõ‚Ì—Ê‚ð’²®‚·‚é‚½‚ß‚Ì•Ï”


/*!
* @brief	‹P“x‚ðŒvŽZB
*/
float CalcLuminance(float3 color)
{
	float luminance = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f)) + g_luminance;
	if (luminance > 1.0f){
		luminance = 1.0f / luminance;
	}
	else{
		luminance = 0.0f;
	}
	return luminance;
}