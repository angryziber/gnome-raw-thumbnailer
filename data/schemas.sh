#!/bin/sh

OWNER=raw-thumbnailer
COMMAND="$2/raw-thumbnailer -s %s %u %o"

. `dirname $0`/mime-functions.sh

upd_schema()
{
        echo "gconftool-2 --set --type $TYPE /desktop/gnome/thumbnailers/$NAME \"$DEFAULT\"" 1>&2
}


schema()
{
	echo "        <schema>";
	echo "            <key>/schemas/desktop/gnome/thumbnailers/$NAME</key>";
	echo "            <applyto>/desktop/gnome/thumbnailers/$NAME</applyto>";
	echo "            <owner>$OWNER</owner>";
	echo "            <type>$TYPE</type>";
	echo "            <default>$DEFAULT</default>";
	echo "            <locale name=\"C\">";
	echo "                <short></short>";
	echo "                <long></long>";
	echo "            </locale>";
	echo "        </schema>";

	upd_schema;
}


get_mimetypes $1;

echo "<gconfschemafile>";
echo "    <schemalist>";

for i in $MIMETYPES ; do
	DIR=`echo $i | sed 's,/,@,' | sed 's,+,@,'`

	NAME="$DIR/enable";
	TYPE="bool";
	DEFAULT="true";
	schema;

	NAME="$DIR/command";
	TYPE="string";
	DEFAULT="$COMMAND";
	schema;
done

echo "    </schemalist>";
echo "</gconfschemafile>"
    
