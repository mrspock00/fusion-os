/*
 * Copyright 2005-2014, Haiku, Inc.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Augustin Cavalier <waddlesplash>
 *		DarkWyrm <bpmagic@columbus.rr.com>
 *		René Gollent
 *		Wim van der Meer <WPJvanderMeer@gmail.com>
 */
#ifndef CREDITS_H
#define CREDITS_H


typedef struct {
	const char* languageCode;
	const char* names;
} Translation;


static const Translation kTranslations[] = {
#if 0
	{ "ar",
		"Khaled Berraoui (khallebal)\n"
		"Kendhia\n"
		"softity\n"
	},
#endif
	{ "be",
		"Aliya\n"
		"Michael Bulash\n"
		"Siaržuk Žarski\n"
	},
	{ "bg",
		"Ognyan Valeri Angelov\n"
		"Росен Арабаджиев\n"
		"cssvb94\n"
		"naydef\n"
	},
	{ "ca",
		"Paco Rivière\n"
	},
	{ "zh",
		"Dong Guangyu\n"
		"Pengfei Han (kurain)\n"
		"Don Liu\n"
		"adcros\n"
		"cirno\n"
		"dgy18787\n"
		"hlwork\n"
	},
	{ "hr",
		"Ivica Koli\n"
		"Zlatko Sehanović\n"
		// "taos\n" bugfixes only, no actual translations
		"zvacet\n"
	},
	{ "cs",
		"aXeton\n"
		"Ivan Masár\n"
		"Pavel Drotár\n"
		"Martin Janiczek\n"
		"Matěj Kocián\n"
		"zafan\n"
	},
	{ "da",
		"KapiX\n"
		"Brian Matzon\n"
		"Kristian Poul Herkild\n"
		"sylvester\n"
	},
	{ "nl",
		"Floris Kint\n" // FKint
		"Puck Meerburg\n" // puckipedia
		"Niels Sascha Reedijk\n" //nielx
		"Meanwhile\n"
		"bilstrong5\n"
		"lolacio\n"
		"madhusudansrg4\n"
		"michiel\n"
		"Sintendo\n"
		"Tagliano\n"
		// "taos\n" bugfixes only, no actual translations
		"wrench123456789\n"
	},
	{ "en_ca",
		"Edwin Amsler\n"
		"infamy\n"
		"Simon South\n"
	},
	{ "en_gb",
		"Harsh Vardhan\n"
		"Humdinger\n"
		"Jessica_Lily\n"
		"Richie Nyhus\n"
	},
	{ "eo",
		"Travis D. Reed (Dancxjo)\n"
		"kojoto\n"
	},
	{ "fi",
		"Jorma Karvonen (Karvjorm)\n"
		"Jaakko Leikas (Garjala)\n"
		"Tommi Nieminen (SuperOscar)\n"
		"Niels Sascha Reedijk\n"
		"Slavi Stefanov Sotirov\n"
		"nortti\n"
	},
	{ "fr",
		"Edwin Amsler\n"
		"Yannick Barbel\n"
		"Jean-Loïc Charroud\n"
		"Adrien Destugues (PulkoMandy)\n"
		"Jérôme Duval (korli)\n"
		"François Revol (mmu_man)\n"
		"Niels Sascha Reedijk\n"
		"Florent Revest\n"
		"Harsh Vardhan\n"
		"abda11ah\n"
		"Adeimantos\n"
		"Blazkowitz\n"
		"Loïc\n"
		"roptat\n"
		"Starchaser\n"
		"Wabouz\n"
	},
	{ "fur",
		"Fabio Tomat\n"
	},
	{ "de",
		"Atalanttore\n"
		"Colin Günther\n"
		"Mirko Israel\n"
		"leszek\n"
		"Christian Morgenroth\n"
		"Aleksas Pantechovskis\n"
		"Niels Sascha Reedijk\n"
		"Joachim Seemer (Humdinger)\n"
		"Matthias Spreiter\n"
		"Ivaylo Tsenkov\n"
		"svend\n"
		"taos\n"
	},
	{ "el",
		"Γιάννης Κωνσταντινίδης [Giannis Konstantinidis] (giannisk)\n"
		"Βαγγέλης Μαμαλάκης [Vaggelis Mamalakis]\n"
		"Άλεξ-Π. Νάτσιος [Alex-P. Natsios] (Drakevr)\n"
		"Efstathios Iosifidis\n"
		"Jim Spentzos\n"
		"JamesSP472\n"
		"vasper\n"
	},
	{ "hi",
		"Abhishek Arora\n"
		"Dhruwat Bhagat\n"
		"Jayneil Dalal\n"
		"Atharva Lath\n"
		"Harsh Vardhan\n"
		"unitedroad\n"
	},
	{ "hu",
		"Zsolt Bicskei\n"
		"Róbert Dancsó (dsjonny)\n"
		"Kálmán Kéménczy\n"
		"Zoltán Mizsei (miqlas)\n"
		"Bence Nagy\n"
		// "Reznikov Sergei (Diver)\n" bugfixes only
		"Zoltán Szabó (Bird)\n"
	},
	{ "id",
		"Raefal Dhia\n"
		"Henry Guzman\n"
		"iyank4\n"
		"mazbrili\n"
		"nurasto\n"
	},
	{ "it",
		"Andrea Bernardi\n"
		"Pavlo Bvrda\n"
		"Dario Casalinuovo\n"
		"Francesco Franchina\n"
		"Michele Frau (zuMi)\n"
		"Lorenzo Frezza\n"
		"Mometto Nicola\n"
		"Michael Peppers\n"
		"Daniele Tosti\n"
		"davide.orsi\n"
		"fabiusp98\n"
		"TheClue\n"
		"TropinotoHirto\n"
		"valzant\n"
	},
	{ "ja",
		"Satoshi Eguchi\n"
		"Shota Fukumori\n"
		"hiromu1996\n"
		"Hironori Ichimiya\n"
		"Jorge G. Mare (Koki)\n"
		"Takashi Murai\n"
		"nolze\n"
		"SHINTA\n"
		"thebowseat\n"
		"Hiroyuki Tsutsumi\n"
		"Hiromu Yakura\n"
		"The JPBE.net user group\n"
	},
	{ "ko",
		"soul.lee\n"
	},
	{ "lt",
		"Algirdas Buckus\n"
		"Simonas Kazlauskas\n" //nagisa
		"Rimas Kudelis\n"
	},
#if 0
	{ "fa",
		"Mehran Rahbardar\n"
	},
#endif
	{ "mi",
		"Rob Judd\n"
	},
	{ "nb",
		"Klapaucius\n"
		"petterhj\n"
	},
	{ "pl",
		"Dariusz Knociński\n"
		"Szymon Barczak\n"
		"Przemysław Buczkowski\n"
		"Grzegorz Dąbrowski\n"
		"Hubert Hareńczyk\n" // Hubert
		"Kacper Kasper (KapiX)\n"
		"Krzysztof Miemiec\n"
		"Przemysław Pintal\n"
		"Artur Wyszyński\n"
		"flegmatyk\n"
		"rausman\n"
		"stasinek\n"
		"zzzzzzzzz\n"
	},
	{ "pt",
		"Marcos Alves (Xeon3D)\n"
		"Vasco Costa (gluon)\n"
		"Victor Domingos\n"
		"Adriano Duarte\n"
		"Louis de M.\n"
		"pedrothegameroficialtm\n"
		"zeru\n"
		// "taos\n" bugfixes only, no actual translations
	},
	{ "pt_BR",
		"Cabral Bandeira (beyraq)\n"
		"Victor Domingos\n"
		"Adriano A. Duarte (Sri_Dhryko)\n"
		"Wallace Maia\n"
		"Tiago Matos (tiagoms)\n"
		"Louis de M.\n"
		"Michael Vinícius de Oliveira (michaelvo)\n"
		"Luis Otte\n"
		"Nadilson Santana (nadilsonsantana)\n"
		"dsaito\n"
	},
	{ "ro",
		"Victor Carbune\n"
		"Silviu Dureanu\n"
		"Alexsander Krustev\n"
		"Danca Monica\n"
		"Florentina Mușat\n" // Emrys
		"Dragos Serban\n"
		"Hedeș Cristian Teodor\n"
		"Ivaylo Tsenkov\n"
		"Călinescu Valentin\n"
		"cipri\n"
		"valzant\n"
	},
	{ "ru",
		"Dan Aller\n"
		"Luka Andjelkovic\n"
		"Tatyana Fursic (iceid)\n"
		"StoroZ Gneva\n"
		"Rodastahm Islamov (RISC)\n"
		"Eugene Katashov (mrNoisy)\n"
		"Pavel Kiryukhin\n"
		"Rimas Kudelis\n"
		"Reznikov Sergei (Diver)\n"
		"Michael Smirnov\n"
		"Sergei Sorokin\n"
		"Vladimir Vasilenko\n"
		"Siaržuk Žarski\n"
		"Алексей Мехоношин\n"
		"Ruskidecko\n"
		"Snowfire\n"
		"solarcold\n"
		"i-Demon-i\n"
		// "taos\n" bugfixes only, no actual translations
	},
	{ "sk",
		"Bachoru\n"
		"Ivan Masár\n"
	},
	{ "sl",
		"Matej Horvat\n"
	},
#if 0
	{ "sr",
		"Luka Andjelkovic\n"
		"Nikola Miljkovic\n"
	},
#endif
	{ "es",
		"Pedro Arregui\n"
		"José Antonio Barranquero\n"
		"Zola Bridges\n"
		"Nicolás C (CapitanPico)\n"
		"Oscar Carballal (oscarcp)\n"
		"Dario de la Cruz\n"
		"Francisco Gómez\n"
		"Luis Gustavo Lira\n"
		"Louis de M.\n"
		"Victor Madariaga\n"
		"Remy Matos\n"
		"César Ortiz Pantoja (ccortiz)\n"
		"Miguel Zúñiga González (miguel~1.mx)\n"
		"Virgilio Leonardo Ruilova\n"
		"algrun\n"
		"asak_lemon\n"
		"c-sanchez\n"
		"EdiNOS\n"
		"espectalll123\n"
		"fatigatti\n"
		"grifus\n"
		"jdari\n"
		"jjpx\n"
		"mpescador\n"
		"OscarL\n"
		"ruilovacastillo\n"
		"valzant\n"
		"zerabat\n"
		// "taos\n" bugfixes only, no actual translations
	},
	{ "sv",
		"Patrik Gissberg\n"
		"Johan Holmberg\n"
		"Theo Knez\n"
		"Fredrik Modéen\n"
		"Jimmy Olsson (phalax)\n"
		"Jonas Sundström\n"
		"Anders Trobäck\n"
		"Victor Widell\n"
	},
#if 0
	{ "tr",
		"Hüseyin Aksu\n"
		"Halil İbrahim Azak\n"
		"Aras Ergus\n"
		"Mustafa Gökay\n"
		"Enes Burhan Kuran\n"
		"Ali Rıza Nazlı\n"
		"Anıl Özbek\n"
		"Sinan Talebi\n"
		"csakirt\n"
		"Hezarfen\n"
		"interlude\n"
		"Kardanadam\n"
		"ocingiler\n"
		"yakup\n"
	},
#endif
	{ "uk",
		"Pavlo Bvrda\n"
		"Mariya Pilipchuk\n"
		"Alex Rudyk (totish)\n"
		// "Reznikov Sergei (Diver)\n" bugfixes only
		"Oleg Varunok\n"
		"axeller\n"
		"neiron13\n"
	},
};

#define kNumberOfTranslations (sizeof(kTranslations) / sizeof(Translation))

#define kCurrentMaintainers \
        "Ferhat Gecdogan\n" \
	"\n"

#define kPastMaintainers \
	" \n" \
	"\n"

#define kContributors \
	"\n" \

#define kWebsiteTeam  \
	"\n" \
	"\n"

#define kPastWebsiteTeam  \
	"\n" \
	"\n"


#endif // CREDITS_H
