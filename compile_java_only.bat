mkdir classes
javac -d classes -target 1.5 src/Java/jdw/graphics/*.java src/Java/jdw/util/*.java src/Java/jdw/demo/*.java
jar cvf jdw.jar bin -C classes jdw 1>NUL
