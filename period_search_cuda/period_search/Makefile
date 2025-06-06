# This should work on Linux.  Modify as needed for other platforms.

# READ ME FIRST:
#
# To keep constinstance of the repository and not be relying to a hardcoded paths please create symbolic links that will be outside the path under
# versioning controll and this way will not brake anyones environment setups.
# From your /home/[YOUR_USERNAME] direcory run: 
#	ln -s /usr/local/cuda-12.8 cuda-12.8
#	ln -s /[your_bpinc_path] boinc_src 
# or change those paths acordingly to your setup.
#

# DO-NOT-CHANGE!
BOINC_DIR = ../../../boinc_src
BOINC_API_DIR = $(BOINC_DIR)/api
BOINC_LIB_DIR = $(BOINC_DIR)/lib
CUDA_PATH = /usr/local/cuda-12
# END DO-NOT-CHANGE

COMMON_OBJ = ../../period_search_common
COBJS = $(patsubst $(COMMON_OBJ)/%.O)

CXXFLAGS = -g -O3 \
	-I$(BOINC_DIR) \
	-I$(BOINC_LIB_DIR) \
	-I$(BOINC_API_DIR) \
	-I$(CUDA_PATH)/include \
	-I ../../period_search_common \
	-L /usr/X11R6/lib \
	-L.

# There were two cases from the legacy code - first was using --maxrregcount=80 which is very ineficient on block occupancy.
# After precise wxamination on results from NVIDIA Nsight Performance Analysis both are set to 32 now. This way we achieve 100% block occupancy (most of the time).
# 28 APR 2020 - Temporaryly '--maxrregcount' is set back to '80' for the NVCCFLAGS until further research proves it needs to be set back to '32' 

# CUDA 5.5: NVCCFLAGS = --ptxas-options=-v --generate-code arch=compute_20,code=sm_20 --generate-code arch=compute_20,code=sm_21 --generate-code arch=compute_30,code=sm_30 --generate-code arch=compute_35,code=sm_35 --generate-code arch=compute_35,code=compute_35 --maxrregcount=80 -rdc=true
# CUDA 10.2 - CC: NVCCFLAGS = compute_30,sm_30;compute_35,sm_35;compute_37,sm_37;compute_50,sm_50;compute_52,sm_52;compute_60,sm_60;compute_61,sm_61;compute_70,sm_70;compute_75,sm_75
# CUDA 11.1 - CC: NVCCFLAGS = compute_52,sm_52;compute_60,sm_60;compute_61,sm_61;compute_70,sm_70;compute_75,sm_75;compute_80,sm_80;compute_86,sm_86
# CUDA 11.8 - CC: NVCCFLAGS = --ptxas-options=-v --generate-code arch=compute_52,code=sm_52 --generate-code arch=compute_60,code=sm_60 --generate-code arch=compute_61,code=sm_61 --generate-code arch=compute_70,code=sm_70 --generate-code arch=compute_75,code=sm_75 --generate-code arch=compute_80,code=sm_80 --generate-code arch=compute_86,code=sm_86 --maxrregcount=80 -rdc=tru
# CUDA 12.8 - CC:
NVCCFLAGS = -O3 --extra-device-vectorization  --ptxas-options=-v -O3 --extra-device-vectorization --generate-code arch=compute_50,code=sm_50 --generate-code arch=compute_52,code=sm_52 \
	--generate-code arch=compute_53,code=sm_53 --generate-code arch=compute_60,code=sm_60 --generate-code arch=compute_61,code=sm_61 --generate-code arch=compute_62,code=sm_62 \
	--generate-code arch=compute_70,code=sm_70 --generate-code arch=compute_72,code=sm_72 --generate-code arch=compute_75,code=sm_75 --generate-code arch=compute_80,code=sm_80 \
	--generate-code arch=compute_86,code=sm_86 --generate-code arch=compute_87,code=sm_87 --generate-code arch=compute_89,code=sm_89 --generate-code arch=compute_90,code=sm_90 --maxrregcount=255 -rdc=true

CC=g++
CXX=g++
OPTFLAGS=-O3
CFLAGS=$(OPTFLAGS) -Wall
LDFLAGS=-lm -lrt -Xlinker -rpath .


MY_LIBS=trifac.o areanorm.o sphfunc.o ellfit.o ludcmp.o lubksb.o mrqmin.o mrqcof.o\
		curv.o blmatrix.o conv.o covsrt.o matrix.o bright.o memory.o\
	$(COMMON_OBJ)/dot_product.o $(COMMON_OBJ)/LcHelpers.o gauss_errc.o pscuda.device-link.o start_CUDA.o curve2_CUDA.o Start.o\
	ComputeCapability.o 

PROGS = period_search_BOINC_cuda12000

all: $(PROGS)

libstdc++.a:
	ln -s `g++ -print-file-name=libstdc++.a`

libcudart.so:
	ln -s $(CUDA_PATH)/lib64/libcudart.so libcudart.so

libcuda.so:
	ln -s $(CUDA_PATH)/lib64/stubs/libcuda.so libcuda.so

libnvidia-ml.so:
	ln -f -s $(CUDA_PATH)/lib64/stubs/libnvidia-ml.so libnvidia-ml.so

clean:
	/bin/rm -f $(PROGS) *.o libstdc++.a libcudart.so libcuda.so

distclean:
	/bin/rm -f $(PROGS) *.o libstdc++.a libcudart.so libcuda.so

install: period_search_BOINC_cuda12000

# specify library paths explicitly (rather than -l)
# because otherwise you might get a version in /usr/lib etc.

period_search_BOINC_cuda12000: period_search_BOINC.o $(MY_LIBS) libstdc++.a $(BOINC_API_DIR)/libboinc_api.a $(BOINC_LIB_DIR)/libboinc.a libcudart.so libcuda.so libnvidia-ml.so
	$(CXX) $(CXXFLAGS) -o $@ $(MY_LIBS) $< libstdc++.a -pthread \
	$(BOINC_API_DIR)/libboinc_api.a libcudart.so libcuda.so libnvidia-ml.so \
	$(BOINC_LIB_DIR)/libboinc.a $(LDFLAGS)

curve2_CUDA.o: curve2_CUDA.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile curve2_CUDA.cu

ComputeCapability.o: ComputeCapability.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile ComputeCapability.cu

start_CUDA.o: start_CUDA.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile start_CUDA.cu

Start.o: Start.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile Start.cu

blmatrix.o: blmatrix.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile blmatrix.cu

bright.o: bright.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile bright.cu

conv.o: conv.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile conv.cu

curv.o: curv.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile curv.cu

gauss_errc.o: gauss_errc.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile gauss_errc.cu

mrqcof.o: mrqcof.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile mrqcof.cu

mrqmin.o: mrqmin.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile mrqmin.cu

matrix.o: matrix.cu
	$(CUDA_PATH)/bin/nvcc --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
		--compile matrix.cu

pscuda.device-link.o: start_CUDA.o Start.o blmatrix.o bright.o conv.o curv.o gauss_errc.o mrqcof.o mrqmin.o matrix.o curve2_CUDA.o ComputeCapability.o
	$(CUDA_PATH)/bin/nvcc -dlink -o pscuda.device-link.o --compiler-bindir=/usr/bin/gcc $(OPTIMIZATION) \
		$(NVCCFLAGS) \
		--compiler-options="$(CXXFLAGS)" \
	start_CUDA.o Start.o blmatrix.o bright.o conv.o curv.o gauss_errc.o mrqcof.o mrqmin.o matrix.o curve2_CUDA.o ComputeCapability.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<
