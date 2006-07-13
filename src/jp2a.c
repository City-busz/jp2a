/*
 * Copyright (C) 2006 Christian Stigen Larsen, http://csl.sublevel3.org
 * Distributed under the GNU General Public License (GPL) v2 or later.
 *
 * Project homepage on http://jp2a.sf.net
 *
 * $Id$
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include "jp2a.h"
#include "options.h"

int main(int argc, char** argv) {
	int r = parse_options(argc, argv);
	if ( r >= 0 ) return r;

	int n;
	for ( n=1; n<argc; ++n ) {

		// skip options
		if ( argv[n][0]=='-' && argv[n][1] )
			continue;

		// read from stdin
		if ( argv[n][0]=='-' && !argv[n][1] ) {
			decompress(stdin);
			continue;
		}

		#ifdef FEAT_CURL
		if ( is_url(argv[n]) ) {

			if ( verbose )
				fprintf(stderr, "URL: %s\n", argv[n]);

			int fd = curl_download(argv[n], debug);

			if ( fd < 0 )
				return 1;

			FILE *fr = fdopen(fd, "rb");

			if ( !fr ) {
				fprintf(stderr, "Could not fdopen read pipe\n");
				return 1;
			}

			decompress(fr);
			fclose(fr);
			close(fd);

			continue;
		}
		#endif

		// read files
		FILE *fp = fopen(argv[n], "rb");

		if ( fp ) {
			if ( verbose )
				fprintf(stderr, "File: %s\n", argv[n]);

			decompress(fp);
			fclose(fp);

			continue;

		} else {
			fprintf(stderr, "Can't open %s\n", argv[n]);
			return 1;
		}
	}

	return 0;
}
