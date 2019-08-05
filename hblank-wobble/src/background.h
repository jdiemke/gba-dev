
/*{{BLOCK(background)                                                   */

/*======================================================================*/
/*                                                                      */
/* background, 240x160@8,                                               */
/* Transparent palette entry: 0.                                        */
/* + palette 256 entries, not compressed                                */
/* + 386 tiles (t|f|p reduced) not compressed                           */
/* + regular map (flat), not compressed, 30x20                          */
/* Total size: 512 + 24704 + 1200 = 26416                               */
/*                                                                      */
/* Time-stamp: 2013-04-23, 01:08:23                                     */
/* Exported by Cearn's GBA Image Transmogrifier                         */
/* ( http://www.coranac.com/projects/#grit )                            */
/*                                                                      */
/*======================================================================*/

#ifndef GRIT_BACKGROUND_H
#define GRIT_BACKGROUND_H

#define backgroundTilesLen 24704
extern const unsigned short backgroundTiles[12352];

#define backgroundMapLen 1200
extern const unsigned short backgroundMap[600];

#define backgroundPalLen 512
extern const unsigned short backgroundPal[256];

#endif /* GRIT_BACKGROUND_H                                             */

/*}}BLOCK(background)                                                   */
