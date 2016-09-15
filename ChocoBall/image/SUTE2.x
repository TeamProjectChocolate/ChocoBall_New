xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 75;
 2.76008;0.84632;-50.01764;,
 3.04343;0.84632;-50.03624;,
 3.04342;-2.11853;-50.03624;,
 2.76008;-1.81424;-50.01764;,
 0.00000;-1.81424;-50.01764;,
 3.04342;-2.11853;-50.03624;,
 0.00000;-2.11853;-50.03624;,
 -2.76008;-1.81424;-50.01764;,
 -3.04343;-2.11853;-50.03624;,
 -3.04343;0.84632;-50.03624;,
 -2.76008;0.84632;-50.01764;,
 -3.04343;-2.11853;-50.03624;,
 3.04342;-2.11853;-0.03755;,
 0.00000;-2.11853;-0.03755;,
 0.00000;-2.11853;-50.03624;,
 3.04342;-2.11853;-50.03624;,
 3.04343;0.84632;-50.03624;,
 3.04343;0.84632;-0.03755;,
 -3.04343;-2.11853;-0.03755;,
 -3.04343;-2.11853;-50.03624;,
 3.04343;0.84632;-0.03755;,
 3.04343;0.84632;-50.03624;,
 2.76008;0.84632;-50.01764;,
 2.81043;0.84635;0.06031;,
 -3.04343;0.84632;-0.03755;,
 -3.04343;0.84632;-50.03624;,
 2.81043;0.84635;0.06031;,
 2.76008;0.84632;-50.01764;,
 2.76008;-1.81424;-50.01764;,
 2.77272;-1.81393;0.07592;,
 -3.04343;0.84632;-0.03755;,
 -2.76008;0.84632;-0.02900;,
 -2.76008;0.84632;-50.01764;,
 -3.04343;0.84632;-50.03624;,
 0.00000;-1.81424;-50.01764;,
 0.00000;-1.81424;-0.02900;,
 -2.76008;0.84632;-0.02900;,
 -2.76008;-1.81424;-0.02900;,
 -2.76008;-1.81424;-50.01764;,
 -2.76008;0.84632;-50.01764;,
 38.10436;-1.83341;35.55156;,
 38.29736;-2.11963;35.35593;,
 38.38123;0.84434;35.32122;,
 38.17980;0.84437;35.52035;,
 36.15270;-1.83077;37.50168;,
 36.14534;-2.11947;37.50624;,
 34.27646;0.84491;39.42057;,
 34.07718;0.84493;39.62184;,
 33.99331;-2.11930;39.65655;,
 34.20103;-1.82813;39.45179;,
 3.04342;-2.11853;-0.03755;,
 38.29736;-2.11963;35.35593;,
 36.14534;-2.11947;37.50624;,
 0.81921;-2.11835;2.12840;,
 3.04343;0.84632;-0.03755;,
 38.38123;0.84434;35.32122;,
 33.99331;-2.11930;39.65655;,
 -1.33281;-2.11818;4.27870;,
 3.04343;0.84632;-0.03755;,
 38.17980;0.84437;35.52035;,
 34.07718;0.84493;39.62184;,
 -1.24895;0.84695;4.24399;,
 38.10436;-1.83341;35.55156;,
 38.17980;0.84437;35.52035;,
 -1.24895;0.84695;4.24399;,
 34.27646;0.84491;39.42057;,
 -1.04255;0.84692;4.04984;,
 0.83368;-1.81098;2.13095;,
 36.15270;-1.83077;37.50168;,
 -1.04255;0.84692;4.04984;,
 34.27646;0.84491;39.42057;,
 34.20103;-1.82813;39.45179;,
 -1.11799;-1.80834;4.08106;,
 0.83368;-1.81098;2.13095;,
 0.00000;-1.81424;-0.02900;;
 
 35;
 4;0,1,2,3;,
 4;4,3,5,6;,
 4;7,8,9,10;,
 4;6,11,7,4;,
 4;12,13,14,15;,
 4;12,15,16,17;,
 4;13,18,19,14;,
 4;20,21,22,23;,
 4;18,24,25,19;,
 4;26,27,28,29;,
 4;30,31,32,33;,
 4;29,28,34,35;,
 4;36,37,38,39;,
 4;35,34,38,37;,
 4;40,41,42,43;,
 4;44,45,41,40;,
 4;46,47,48,49;,
 4;49,48,45,44;,
 4;50,51,52,53;,
 4;50,54,55,51;,
 4;53,52,56,57;,
 4;58,23,59,42;,
 4;57,56,60,61;,
 4;26,29,62,63;,
 4;64,47,65,66;,
 4;29,67,68,62;,
 4;69,70,71,72;,
 4;67,72,71,68;,
 3;73,29,74;,
 4;74,37,72,73;,
 4;72,37,36,69;,
 4;31,30,64,66;,
 4;24,18,57,61;,
 3;12,53,13;,
 4;13,53,57,18;;
 
 MeshMaterialList {
  1;
  35;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.525000;0.525000;0.525000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.512000;0.512000;0.512000;;
   TextureFilename {
    "image\\Stex.png";
   }
  }
 }
 MeshNormals {
  49;
  -0.065515;0.000000;-0.997852;,
  -0.032790;0.030542;-0.998995;,
  0.000000;0.061023;-0.998136;,
  0.065515;0.000000;-0.997852;,
  0.032790;0.030542;-0.998995;,
  0.000000;-1.000000;0.000043;,
  0.000000;-1.000000;0.000042;,
  1.000000;-0.000002;-0.000000;,
  0.000001;-1.000000;0.000041;,
  0.000082;1.000000;-0.000023;,
  -0.981300;0.007795;0.192330;,
  -0.923983;0.015291;0.382127;,
  0.981086;-0.007832;-0.193416;,
  0.703168;-0.011545;0.710930;,
  0.704991;-0.009208;0.709156;,
  0.706806;-0.006843;0.707374;,
  0.710436;-0.011845;0.703662;,
  0.708626;-0.009331;0.705523;,
  -0.000056;-1.000000;0.000024;,
  -0.000027;-1.000000;0.000054;,
  0.707838;-0.014143;-0.706233;,
  -0.000026;-1.000000;0.000053;,
  -0.833032;0.022225;0.552778;,
  0.000049;1.000000;-0.000093;,
  0.832417;-0.022271;-0.553702;,
  0.000459;1.000000;-0.000942;,
  -0.000042;1.000000;-0.000735;,
  0.000001;1.000000;-0.000073;,
  0.000000;-1.000000;0.000000;,
  0.000062;1.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.999975;0.007091;0.000629;,
  0.000000;1.000000;0.000000;,
  0.000293;1.000000;-0.000641;,
  -0.000056;1.000000;-0.000003;,
  -0.000028;1.000000;-0.000002;,
  1.000000;0.000000;0.000000;,
  -0.000028;1.000000;-0.000725;,
  -0.000055;-1.000000;0.000023;,
  -0.000054;-1.000000;0.000022;,
  0.000101;1.000000;-0.000045;,
  -0.707340;0.028277;0.706307;,
  -0.708116;0.021176;0.705778;,
  0.000097;1.000000;-0.000040;,
  0.000974;1.000000;-0.000415;,
  0.000990;0.999999;-0.000433;,
  0.707336;-0.028306;-0.706311;,
  0.000451;1.000000;-0.000924;,
  0.000957;0.999999;-0.000396;;
  35;
  4;0,0,1,1;,
  4;2,1,1,2;,
  4;4,4,3,3;,
  4;2,4,4,2;,
  4;5,6,28,28;,
  4;7,7,7,7;,
  4;6,8,28,28;,
  4;29,29,29,9;,
  4;10,10,30,30;,
  4;11,31,31,11;,
  4;27,27,32,32;,
  4;33,34,35,26;,
  4;12,12,36,36;,
  4;26,35,32,37;,
  4;14,14,13,13;,
  4;15,15,14,14;,
  4;16,16,17,17;,
  4;17,17,15,15;,
  4;18,18,38,19;,
  4;20,20,20,20;,
  4;19,38,39,21;,
  4;40,9,40,40;,
  4;22,41,41,22;,
  4;11,11,42,42;,
  4;23,43,43,23;,
  4;33,25,44,45;,
  4;24,46,46,24;,
  4;25,47,48,44;,
  3;25,33,26;,
  4;26,37,47,25;,
  4;24,12,12,24;,
  4;27,27,23,23;,
  4;10,10,22,22;,
  3;5,19,6;,
  4;6,19,21,8;;
 }
 MeshTextureCoords {
  75;
  0.494790;0.005120;,
  0.979920;0.005120;,
  0.979910;5.008910;,
  0.494790;4.498340;,
  -4.230750;4.498340;,
  0.979930;5.008910;,
  -4.230750;5.008910;,
  0.494790;4.498340;,
  0.979910;5.008910;,
  0.979920;0.005120;,
  0.494790;0.005120;,
  0.979930;5.008910;,
  34.521099;2.012290;,
  34.521099;0.003960;,
  0.003220;0.003960;,
  0.003220;2.012290;,
  0.003220;0.003960;,
  34.521099;0.003960;,
  34.521099;2.012290;,
  0.003220;2.012290;,
  0.979920;5.755160;,
  0.979920;11.505200;,
  0.486830;11.505200;,
  0.486830;5.755160;,
  34.521099;0.003960;,
  0.003220;0.003960;,
  34.521099;0.003960;,
  0.003220;0.003960;,
  0.003220;2.012290;,
  34.521099;2.012290;,
  0.979920;5.755160;,
  0.486830;5.755160;,
  0.486830;11.505200;,
  0.979920;11.505200;,
  0.003220;0.003960;,
  34.521099;0.003960;,
  34.521099;0.003960;,
  34.521099;2.012290;,
  0.003220;2.012290;,
  0.003220;0.003960;,
  0.494790;4.498340;,
  0.979910;5.008910;,
  0.979920;0.005120;,
  0.494790;0.005120;,
  -4.230750;4.498340;,
  -4.230750;5.008910;,
  0.494790;0.005120;,
  0.979920;0.005120;,
  0.979910;5.008910;,
  0.494790;4.498340;,
  34.521099;2.012290;,
  69.038902;2.012290;,
  69.038902;0.003960;,
  34.521099;0.003960;,
  34.521099;0.003960;,
  69.038902;0.003960;,
  69.038902;2.012290;,
  34.521099;2.012290;,
  0.979920;5.755160;,
  0.486830;0.005120;,
  69.038902;0.003960;,
  34.521099;0.003960;,
  69.038902;2.012290;,
  69.038902;0.003960;,
  0.979920;5.755160;,
  0.486830;0.005120;,
  0.486830;5.755160;,
  34.521099;0.003960;,
  69.038902;0.003960;,
  34.521099;0.003960;,
  69.038902;0.003960;,
  69.038902;2.012290;,
  34.521099;2.012290;,
  34.521099;2.012290;,
  34.521099;2.012290;;
 }
 MeshVertexColors {
  75;
  0;1.000000;1.000000;1.000000;1.000000;,
  1;1.000000;1.000000;1.000000;1.000000;,
  2;1.000000;1.000000;1.000000;1.000000;,
  3;1.000000;1.000000;1.000000;1.000000;,
  4;1.000000;1.000000;1.000000;1.000000;,
  5;1.000000;1.000000;1.000000;1.000000;,
  6;1.000000;1.000000;1.000000;1.000000;,
  7;1.000000;1.000000;1.000000;1.000000;,
  8;1.000000;1.000000;1.000000;1.000000;,
  9;1.000000;1.000000;1.000000;1.000000;,
  10;1.000000;1.000000;1.000000;1.000000;,
  11;1.000000;1.000000;1.000000;1.000000;,
  12;1.000000;1.000000;1.000000;1.000000;,
  13;1.000000;1.000000;1.000000;1.000000;,
  14;1.000000;1.000000;1.000000;1.000000;,
  15;1.000000;1.000000;1.000000;1.000000;,
  16;1.000000;1.000000;1.000000;1.000000;,
  17;1.000000;1.000000;1.000000;1.000000;,
  18;1.000000;1.000000;1.000000;1.000000;,
  19;1.000000;1.000000;1.000000;1.000000;,
  20;1.000000;1.000000;1.000000;1.000000;,
  21;1.000000;1.000000;1.000000;1.000000;,
  22;1.000000;1.000000;1.000000;1.000000;,
  23;1.000000;1.000000;1.000000;1.000000;,
  24;1.000000;1.000000;1.000000;1.000000;,
  25;1.000000;1.000000;1.000000;1.000000;,
  26;1.000000;1.000000;1.000000;1.000000;,
  27;1.000000;1.000000;1.000000;1.000000;,
  28;1.000000;1.000000;1.000000;1.000000;,
  29;1.000000;1.000000;1.000000;1.000000;,
  30;1.000000;1.000000;1.000000;1.000000;,
  31;1.000000;1.000000;1.000000;1.000000;,
  32;1.000000;1.000000;1.000000;1.000000;,
  33;1.000000;1.000000;1.000000;1.000000;,
  34;1.000000;1.000000;1.000000;1.000000;,
  35;1.000000;1.000000;1.000000;1.000000;,
  36;1.000000;1.000000;1.000000;1.000000;,
  37;1.000000;1.000000;1.000000;1.000000;,
  38;1.000000;1.000000;1.000000;1.000000;,
  39;1.000000;1.000000;1.000000;1.000000;,
  40;1.000000;1.000000;1.000000;1.000000;,
  41;1.000000;1.000000;1.000000;1.000000;,
  42;1.000000;1.000000;1.000000;1.000000;,
  43;1.000000;1.000000;1.000000;1.000000;,
  44;1.000000;1.000000;1.000000;1.000000;,
  45;1.000000;1.000000;1.000000;1.000000;,
  46;1.000000;1.000000;1.000000;1.000000;,
  47;1.000000;1.000000;1.000000;1.000000;,
  48;1.000000;1.000000;1.000000;1.000000;,
  49;1.000000;1.000000;1.000000;1.000000;,
  50;1.000000;1.000000;1.000000;1.000000;,
  51;1.000000;1.000000;1.000000;1.000000;,
  52;1.000000;1.000000;1.000000;1.000000;,
  53;1.000000;1.000000;1.000000;1.000000;,
  54;1.000000;1.000000;1.000000;1.000000;,
  55;1.000000;1.000000;1.000000;1.000000;,
  56;1.000000;1.000000;1.000000;1.000000;,
  57;1.000000;1.000000;1.000000;1.000000;,
  58;1.000000;1.000000;1.000000;1.000000;,
  59;1.000000;1.000000;1.000000;1.000000;,
  60;1.000000;1.000000;1.000000;1.000000;,
  61;1.000000;1.000000;1.000000;1.000000;,
  62;1.000000;1.000000;1.000000;1.000000;,
  63;1.000000;1.000000;1.000000;1.000000;,
  64;1.000000;1.000000;1.000000;1.000000;,
  65;1.000000;1.000000;1.000000;1.000000;,
  66;1.000000;1.000000;1.000000;1.000000;,
  67;1.000000;1.000000;1.000000;1.000000;,
  68;1.000000;1.000000;1.000000;1.000000;,
  69;1.000000;1.000000;1.000000;1.000000;,
  70;1.000000;1.000000;1.000000;1.000000;,
  71;1.000000;1.000000;1.000000;1.000000;,
  72;1.000000;1.000000;1.000000;1.000000;,
  73;1.000000;1.000000;1.000000;1.000000;,
  74;1.000000;1.000000;1.000000;1.000000;;
 }
}
