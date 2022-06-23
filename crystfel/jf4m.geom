; PSI JF4M 

; Camera length (in m) and photon energy (eV)
clen = 0.075
photon_energy = 12400

flag_lessthan=-30000

; older, change it to adu_per_eV and set it to one over the photon energy in eV
adu_per_photon = 1
res = 13333.3   ; 75 micron pixel size

rigid_group_collection_0 = 0
rigid_group_0 = 0

; These lines describe the data layout for the Eiger native multi-event files
dim0 = %
dim1 = ss
dim2 = fs
data = /entry/data/data

; Mask out strips between panels
bad_v0/min_fs = 1030
bad_v0/min_ss = 0
bad_v0/max_fs = 1037
bad_v0/max_ss = 2163

bad_h0/min_fs = 0
bad_h0/min_ss = 514
bad_h0/max_fs = 2067
bad_h0/max_ss = 549

bad_h1/min_fs = 0
bad_h1/min_ss = 1064
bad_h1/max_fs = 2067
bad_h1/max_ss = 1099

bad_h2/min_fs = 0
bad_h2/min_ss = 1614
bad_h2/max_fs = 2067
bad_h2/max_ss = 1649

; Uncomment these lines if you have a separate bad pixel map (recommended!)
;mask_file = eiger-badmap.h5
mask = /entry/instrument/detector/pixel_mask
mask_good = 0x0
mask_bad = 0xFF

; corner_{x,y} set the position of the corner of the detector (in pixels)
; relative to the beam
0/min_fs = 0
0/min_ss = 0
0/max_fs = 2067
0/max_ss = 2163
0/corner_x =  -1090.72
0/corner_y =  -1137.24
0/fs = +1.000000x +0.000373y
0/ss = -0.000373x +1.000000y




0/coffset = -0.000001
