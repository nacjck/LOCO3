cd makefiles
make clean_bin -f descompresor.mk
make -f descompresor.mk
make clean_bin -f compresor.mk
make -f compresor.mk
mv compresorLOCO ../
mv descompresorLOCO ../
