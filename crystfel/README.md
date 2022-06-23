# CrystFEL test 

This test requires recent version of CrystFEL with Xgandalf indexing algorithm. Tested with CrystFEL 0.10.0.

## Generate HDF5 file
To generate HDF5 you need first to compile `HDFDatasetWriteTest`. From main directory:

```
mkdir build
cd build
cmake3 ..
make HDF5DatasetWriteTest
```

Go back to main directory and:
```
cd crystfel
HDF5DATASET_WRITE_TEST_SWMR=1 ../build/tests/HDF5DatasetWriteTest ../tests/test_data/compression_benchmark.h5 100
```
This will generate HDF5 with lysozyme data measured with JUNGFRAU 4 Mpixel long time ago.

## Analyze with CrystFEL
To process the generated file with CrystFEL, you need to run the following command:
```
indexamajig -g jf4m.geom -i writing_test.lst -o writing_test.stream --indexing=xgandalf
```
Hopefully you see the following outcome at the end:
```
Final: 100 images processed, 100 hits (100.0%), 100 indexable (100.0% of hits, 100.0% overall), 100 crystals.
```
In case of issues, you can also try providing unit cell information:
```
indexamajig -g jf4m.geom -i writing_test.lst -o writing_test.stream --indexing=xgandalf -p 6G8A.pdb
```
