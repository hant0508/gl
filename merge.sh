#!/bin/bash

# Скрипт циклически читает все файлы в папке с расширением .cpp и ищет их хедеры,
# т.е. файлы с тем же именем, но расширением .hpp. Записывает в один файл
# out.cpp сначала содержимое хедера (при его наличии), а потом содержимое самого
# исходника. Кроме того, из файлов удаляются все комментарии, если подряд идут
# несколько пустых строк, то оставляет одну. Перед началом каждого нового файла
# пишет его заголовок комментарием.
# Если какие-то файлы вклеивать не надо, они явно исключаются в начале цикла,
# при необходимости вклеить дополнительные файлы (какой-нибудь
# отдельный .hpp или .txt.), можно использовать add_file перед циклом или после него.
# Исключение out.cpp в начале цикла обязательно!

echo '' > out.cpp

add_file ()
{
	sed -r ':a; s%(.*)/\*.*\*/%\1%; ta; /\/\*/ !b; N; ba' $1 > out.tmp
	echo  $'\n' '/***' $1 '***/' $'\n' >> out.cpp
	cat out.tmp >> out.cpp
}

for i in `ls *.cpp`; do
	if [ "$i" != "out.cpp" ] && [ "$i" != "lodepng.cpp" ] && [ "$i" != "glcanvas.cpp" ]
	then
		if test -f ${i%%.*}.hpp
		then
			add_file ${i%%.*}.hpp
		fi

		add_file $i
	fi
done

###

add_file util.hpp

###

sed -i 's/\/\/.*//' out.cpp
sed -i '1,2d' out.cpp
sed -i 's/^ *	*$//' out.cpp
sed -i 's/^	* *$//' out.cpp
sed -i 's///' out.cpp
cat -s out.cpp > out.tmp
cat out.tmp > out.cpp
rm out.tmp

vim out.cpp
