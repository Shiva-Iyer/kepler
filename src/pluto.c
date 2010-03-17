/*
 * pluto.c - Routines to implement an analytical theory for Pluto
 * Copyright (C) 2010 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Reference : "Ephemerides of Pluto between 1700 and 2100 (PLUTO95)"
 *             - J.Chapront, G.Francou
 *             <ftp://cdsarc.u-strasbg.fr/pub/cats/VI/88/>
 */

#include <math.h>
#include <kepler.h>
#include <pluto.h>

/* The secular terms in the theory */
double ax[4] = {98083308510.0,-1465718392.0,11528487809.0,55397965917.0};

double ay[4] = {101846243715.0,57789.0,-5487929294.0,8520205290.0};

double az[4] = {2183700004.0,433209785.0,-4911803413.0,-14029741184.0};

/* Periodic and Poisson terms in the theory */
double cx[106] = {
	-16338582222.0,-5995086437.0,23663880362.0,10304632056.0,-3996936944.0,
	-4136465568.0,1188702881.0,-621434363.0,566898160.0,-75880391.0,576146406.0,
	-659684298.0,451962774.0,-153724334.0,-603163280.0,364764379.0,193062130.0,
	161493959.0,1167349082.0,-1417467887.0,15325240.0,-3624391.0,-587306.0,
	132022.0,-106501.0,228373.0,-95106.0,56299.0,-48339.0,803937.0,
	 -6172744.0,-18962749.0,133022.0,-25964.0,7111.0,-4998.0,
	 32034.0,-29666.0,-1983.0,114.0,191.0,-1063.0,
	 419.0,346.0,5059.0,-81.0,1408.0,2964.0,
	 -5364.0,1509.0,-4924.0,2954.0,2034.0,-5199.0,
	 604.0,-1247.0,4576.0,-350741.0,-4023.0,1147.0,
	 -38.0,-99.0,-11686.0,1129.0,582.0,-83.0,
	 -97.0,431.0,-134.0,-323.0,-292.0,195.0,
	 39068.0,523.0,-1747.0,3135.0,-619.0,-12095.0,
	 6.0,18476.0,-130.0,-438.0,102345278799.0,-9329130892.0,
	 1484339404.0,472660593.0,-581239444.0,1016663241.0,-1054199614.0,
	99039105.0,-52190030.0,-3394173.0,-16529.0,3102430.0,2286.0,-10955.0,
	 -5293.0,-654.0,124.0,-85.0,29.0,418209651.0,
	 -1191875710.0,-823081.0,-558.0,-1091.0
};

double cy[106] = {
	299584895562.0, 75951634908.0,-36135662843.0,18125610071.0,-20398008415.0,
	6125780503.0,-162559485.0,4352425804.0,-3819676998.0,1168107376.0,
	-5041323701.0,4093828501.0,-1727274544.0,134214260.0,5033950069.0,
	-3071449401.0,-1190419055.0,-775881742.0,-5524713888.0,6803228005.0,
	-65675611.0,15155413.0,2009509.0,-389682.0,275571.0,474366.0,132163.0,
	-81550.0,69996.0,-706470.0,4777898.0,-44002785.0,-58735.0,7624.0,
	-1922.0,-729.0,-1733.0,-35642.0,-586.0,-258.0,-368.0,1286.0,-136.0,883.0,
	2673.0,331.0,50.0,178.0,2901.0,-654.0,-8972.0,3034.0,1113.0,570.0,-72.0,
	1950.0,8550.0,1047593.0,-2348.0,313.0,432.0,6765.0,-8240.0,335.0,140.0,
	-833.0,252.0,-210.0,366.0,-920.0,1215.0,-217.0,-17780.0,581.0,-560.0,
	-4131.0,390.0,25613.0,-206.0,1850.0,171.0,-471.0,26437625772.0,
	-12674907683.0,-1067899665.0,-2082744.0,-43195632.0,211912497.0,
	-108307161.0,-63033809.0,-203850703.0,-1672332.0,7136.0,803655.0,-10985.0,
	9126.0,3317.0,-151.0,160.0,138.0,-27.0,-36463065062.0,-5816560445.0,
	1576292.0,-21.0,-295.0
 };

double cz[106] = { 
	98425296138.0,25475793908.0,-18424386574.0,2645968636.0,-5282207967.0,
	3278235471.0,-425422632.0,1526641086.0,-1323182752.0,235873266.0,
	-1617466723.0,1557465867.0,-848586296.0,218182986.0,1636044515.0,
	-1001334243.0,-455739370.0,-348173978.0,-2511254281.0,3062521470.0,
	-32079379.0,7597939.0,1138566.0,-238849.0,192377.0,83169.0,148694.0,
	-92489.0,87116.0,-1281070.0,9950106.0,-25105642.0,-171749.0,31035.0,
	-8648.0,5360.0,-30345.0,11482.0,1322.0,-467.0,96.0,894.0,-381.0,-583.0,
	2525.0,-569.0,226.0,-2039.0,3728.0,-1540.0,42.0,-3144.0,658.0,220.0,
	1848.0,678.0,-7289.0,463291.0,3945.0,-1141.0,-26.0,-10607.0,11458.0,
	-1005.0,120.0,-301.0,135.0,-186.0,118.0,30.0,197.0,-182.0,-8585.0,240.0,
	-226.0,-2049.0,283.0,11109.0,-100.0,-842.0,71.0,-181.0,-22591501373.0,
	-1138977908.0,-782718600.0,-141483824.0,159033355.0,-246222739.0,
	287284767.0,-48002332.0,-41114335.0,578004.0,-8420.0,-766779.0,957.0,
	5780.0,4141.0,417.0,-8.0,65.0,-22.0,-11656050047.0,-1186276469.0,
	1388681.0,201.0,561.0
};

double sx[106] = {
	-308294137468.0,-68820910480.0,28346466257.0,-1755658975.0,7818660837.0,
	-1098895702.0,-1192462299.0,-772129982.0,1061702581.0,-639572722.0,
	1128327488.0,-423570428.0,-175317704.0,251601606.0,-869448807.0,551228298.0,
	87807522.0,-11540541.0,-103236703.0,92638954.0,-3624991.0,1004975.0,
	304396.0,-56532.0,55554.0,-799096.0,56947.0,-48016.0,50599.0,-680660.0,
	5858452.0,38125648.0,-109460.0,18684.0,-5269.0,2771.0,-6814.0,47130.0,
	1192.0,-1387.0,379.0,-612.0,-52.0,813.0,-4354.0,-2275.0,685.0,-1352.0,
	4681.0,-1908.0,-6530.0,8667.0,1675.0,874.0,898.0,965.0,-7124.0,-1145389.0,
	2931.0,-618.0,-34.0,-6562.0,8038.0,-697.0,-8.0,12.0,-267.0,-131.0,304.0,
	-756.0,-103.0,-250.0,19816.0,-596.0,576.0,4122.0,65.0,-27900.0,217.0,-137.0,
	-269.0,531.0,-24338350765.0,11210995713.0,2793567155.0,-776019789.0,
	1528323591.0,-249354416.0,1127608109.0,-667692329.0,-1570766679.0,-9724425.0,
	26552.0,3332520.0,-27607.0,-11696.0,-7297.0,-104.0,-184.0,-455.0,-16.0,
	39813894679.0,3633087275.0,522728.0,-320.0,-1401.0
};

double sy[106] = { 
	-53545027809.0,-8838029861.0,23553788174.0,13775798112.0,-6068121593.0,
	-2853107588.0,750355551.0,-82067770.0,230091832.0,-259838942.0,197944074.0,
	27141006.0,-105334544.0,95175918.0,-139461973.0,80593104.0,-5126842.0,
	-21953793.0,-163767784.0,192436228.0,-2479113.0,561687.0,121909.0,-30275.0,
	16333.0,68105.0,24081.0,-11228.0,667.0,-73047.0,1007089.0,-22814549.0,434.0,
	1013.0,710.0,1100.0,-4598.0,1990.0,564.0,828.0,-1119.0,-1249.0,-597.0,227.0,
	5467.0,801.0,-2029.0,-1892.0,4713.0,-459.0,1757.0,-9303.0,-2357.0,7679.0,
	-2953.0,629.0,5011.0,-333905.0,-2388.0,415.0,139.0,-5726.0,-4583.0,310.0,
	681.0,-107.0,301.0,-525.0,198.0,-379.0,-230.0,-64.0,36069.0,459.0,-1596.0,
	2509.0,-146.0,-11081.0,4.0,15764.0,-147.0,-362.0,117449924600.0,
	-7691661502.0,-4771148239.0,3733883366.0,-7081845126.0,3502526523.0,
	-8115570206.0,3607883959.0,7690328772.0,37384011.0,-164319.0,-2859257.0,
	1593.0,-11997.0,-6476.0,1419.0,34.0,232.0,32.0,2752753498.0,
	-672124207.0,154239.0,-400.0,372.0
};

double sz[106] = { 
	76159403805.0,17987340882.0,-1193982379.0,4828308190.0,-4248985438.0,
	-559147671.0,593594960.0,208799497.0,-249913200.0,115051024.0,
	-282588988.0,135883560.0,23091693.0,-49187976.0,223956575.0,-137344299.0,
	-28188872.0,-2636274.0,-14202661.0,25488216.0,419837.0,-150966.0,
	-64906.0,3719.0,-2226.0,86321.0,-15970.0,16609.0,-15782.0,200300.0,
	-1500491.0,-9161491.0,37481.0,-4616.0,224.0,-1027.0,5220.0,-6976.0,-267.0,
	556.0,-23.0,-711.0,-122.0,-97.0,2440.0,786.0,-806.0,-167.0,-156.0,572.0,
	2532.0,-4582.0,-1178.0,875.0,-558.0,781.0,3230.0,-116132.0,-1440.0,438.0,
	176.0,1072.0,-5850.0,418.0,267.0,60.0,134.0,-85.0,-59.0,112.0,-168.0,-89.0,
	14986.0,190.0,-685.0,1018.0,-48.0,-4807.0,0.0,7066.0,-54.0,-229.0,
	44126663549.0,-5626220823.0,-2536450838.0,1536292657.0,-2916144530.0,
	949074586.0,-2842935040.0,1500396857.0,3415136438.0,19702076.0,-46995.0,
	-5801645.0,33470.0,17674.0,7355.0,199.0,11.0,205.0,33.0,-11127973411.0,
	-1310869292.0,-164753.0,-107.0,284.0
};

/* Table of frequencies */
double fq[106] = { 
	6.45003954767e-5,1.083248054773e-4,1.302772403167e-4,1.64786865996e-4,
	1.935009111902e-4,2.223740247147e-4,3.032575201026e-4,3.259246239385e-4,
	3.564763034914e-4,4.265811293132e-4,4.503959517513e-4,4.638675148284e-4,
	5.009272733421e-4,5.163593863414e-4,5.57882682821e-4,5.882795362847e-4,
	6.450023602974e-4,7.097635821639e-4,7.630643253588e-4,7.740033551209e-4,
	8.385031396726e-4,8.95059160972e-4,9.545118163938e-4,.00102554175696,
	.0010826728325744,.0011680358909203,.0012405125052369,.0012931805883876,
	.0013460706181008,.0014190059530383,.0014394705053002,.0014502634075377,
	.0014992014575181,.0015434430430867,.0016000710611098,.0016562809940875,
	.0017275924266291,.0017454042542465,.0018215079641428,.0018694826929211,
	.0019274630193251,.0020276790928706,.0021822818660433,.002288528985497,
	.002316764637942,.0023445464874575,.0024069306189938,.0024473146628449,
	.0024778027974419,.0025244208011161,.0025682157855485,.0026028617439482,
	.0026544444009919,.0026987455959123,.0027308225916697,.0027735113723168,
	.002872838546403,.0029001725379479,.0029379670182566,.0029750359447782,
	.0031326820696785,.0031822107498712,.0031931048857857,.0032268922327691,
	.0034657232066225,.003783858164567,.0038055149432355,.0038631344783149,
	.0039129259467328,.004031144546251,.004060754200893,.0041490103414206,
	.0043500678052272,.0046321937641054,.005800037672524,.0091460971544658,
	.0091560629947357,.0172021239411871,.0182919855069063,.0279624510118796,
	.034404099617764,.0714245719830324,1.083248054773e-4,1.64786865996e-4,
	3.032575201026e-4,3.259246239385e-4,3.564763034914e-4,5.009272733421e-4,
	5.882795362847e-4,7.097635821639e-4,7.630643253588e-4,8.95059160972e-4,
	.0011680358909203,.0014502634075377,.0017454042542465,.0029001725379479,
	.0031822107498712,.004060754200893,.0043500678052272,.0091460971544658,
	.0279624510118796,1.083248054773e-4,3.032575201026e-4,.0011680358909203,
	.0043500678052272,.0279624510118796
};

/**
 * Calculates the heliocentric rectangular coordinates of Pluto using an
 * analytical model developed at the Bureau des Longitudes (Pluto95).
 *
 * This theory is valid only for dates between Jan 01, 1700 (inclusive) 
 * and Jan 24, 2100 (exclusive).
 *
 * @param[in] tdb TDB to be used for calculations. TT may be used for all but
 * the most exacting applications. Must be between 1700-01-01 and 2100-01-24.
 * @param[out] pos On success, the heliocentric rectangular coordinates of Pluto
 * in AU. The reference frame is the equinox and equator of J2000.
 *
 * @retval SUCCESS If the coordinates were calculated successfully.
 * @retval ERR_INVALID_DATE If tdb was out of range for the theory.
 **/
int pluto_coordinates(struct julian_date *tdb,
		struct rectangular_coordinates *pos)
{
	int i,j;
	double t,x[4],fx,sn,cs;
	struct rectangular_coordinates sec,per;

	t = tdb->date1 + tdb->date2;
	if (t < PLUTO_MIN_DATE || t > PLUTO_MAX_DATE)
		return ERR_INVALID_DATE;

	x[0] = 1;
	x[1] = ((t - PLUTO_MIN_DATE) / 73060.0) - 1;
	x[2] = x[1] * x[1];
	x[3] = x[2] * x[1];

	fx = t - PLUTO_MIN_DATE - 73060.0;

	sec.x = 0;
	sec.y = 0;
	sec.z = 0;
 
	/* Sum up secular terms */
	for (i = 0; i <= 3; i++) {
		sec.x += ax[i] * x[i];
		sec.y += ay[i] * x[i];
		sec.z += az[i] * x[i];
	}

	j = 0;
	per.x = 0;
	per.y = 0;
	per.z = 0;

	/* Sum up periodic & Poisson terms */
	for (i = 0; i < 106; i++) {
		/* Index into x[] for first order Poisson terms */
		if (i == 82)
			j = 1;
		/* Index into x[] for second order Poisson terms */
		if (i == 101)
			j = 2;

		sincos(fq[i] * fx, &sn, &cs);

		per.x += (cx[i] * cs + sx[i] * sn) * x[j];
		per.y += (cy[i] * cs + sy[i] * sn) * x[j];
		per.z += (cz[i] * cs + sz[i] * sn) * x[j];
	}

	pos->x = (per.x + sec.x) / 1e10;
	pos->y = (per.y + sec.y) / 1e10;
	pos->z = (per.z + sec.z) / 1e10;

	return SUCCESS;
}
