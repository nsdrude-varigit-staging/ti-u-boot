#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/global_data.h>
#include <fdt_support.h>
#include <asm/io.h>


#include "AM62-SOM-VIC1040_8Gb_IT_K4A8G165WC-BIWE_DDR_Config_0.09.08.0000/board_ddrReginit.h"

/*
Table 9-5. DDRSS0 Memory Regions
DDR controller registers0x00F308000
DDR PHY independent module registers0x00F30A000
DDR PHY registers0x00F30C000
*/

#define DDR_REG_CTL_START 0x00F308000
#define DDR_REG_PHY_START 0x00F30C000
#define DDR_REG_PI_START 0x00F30A000

int verify_ddr_registers(const char * name, uint32_t *ddr_reg_values, uint32_t start_addr, uint32_t length)
{
    uint32_t *soc_reg_ptr = (uint32_t *) start_addr;
    uint32_t *ddr_reg_ptr = ddr_reg_values;
    int i;
	int ret = 0;
	printf("---%s---\n", name);

    for (i = 0; i < length; i++) {
        if (*soc_reg_ptr != *ddr_reg_ptr) {
            printf("Mismatch at register %s_%d (SOC: 0x%08X, DDR: 0x%08X)\n", name, i, *soc_reg_ptr, *ddr_reg_ptr);
			ret++;
        }
        soc_reg_ptr++;
        ddr_reg_ptr++;
    }

	printf("%s: %d registers do not match SOC registers\n", name, ret);


    return ret; // return number of mismatches
}

void var_add_readonly() {
	DDRSS_ctlReg[0] |= 0x10460000;
	DDRSS_ctlReg[1] |= 0x5D1AF3C3;
	DDRSS_ctlReg[2] |= 0x171A610;
	DDRSS_ctlReg[3] |= 0x40020A11;
	DDRSS_ctlReg[4] |= 0x00052006;
	DDRSS_ctlReg[5] |= 0x02050020;
	DDRSS_ctlReg[6] |= 0x03070101;
}

void var_verify_ddr() {
	int result;
	printf("---------------------------------------------------\n");
	printf("%s\n", __func__);

	// var_add_readonly();

	// Search reference manual for DDR16SS0_CTL_<offset> e.g. DDR16SS0_CTL_167
	result = verify_ddr_registers("DENALI_CTL", DDRSS_ctlReg, DDR_REG_CTL_START, sizeof(DDRSS_ctlReg) / sizeof(uint32_t));
	result = verify_ddr_registers("DENALI_PHY", DDRSS_phyReg, DDR_REG_PHY_START, sizeof(DDRSS_phyReg) / sizeof(uint32_t));
	result = verify_ddr_registers("DENALI_PI", DDRSS_phyIndepReg, DDR_REG_PI_START, sizeof(DDRSS_phyIndepReg) / sizeof(uint32_t));

	printf("---------------------------------------------------\n");
}

void board_k3_adjust_ddr_done() {
	var_verify_ddr();
}

#if 0

Table 9-5. DDRSS0 Memory Regions
DDR controller registers0x00F308000
DDR PHY independent module registers0x00F30A000
DDR PHY registers0x00F30C000

	status = driverdt->writectlconfig(pd, reginitdata.ctl_regs,
					  reginitdata.ctl_regs_offs,
					  LPDDR4_INTR_CTL_REG_COUNT);
	if (!status)
		status = driverdt->writephyindepconfig(pd, reginitdata.pi_regs,
						       reginitdata.pi_regs_offs,
						       LPDDR4_INTR_PHY_INDEP_REG_COUNT);
	if (!status)
		status = driverdt->writephyconfig(pd, reginitdata.phy_regs,
						  reginitdata.phy_regs_offs,
						  LPDDR4_INTR_PHY_REG_COUNT);
	if (status) {

		.writectlconfig			= lpddr4_writectlconfig,
		.writephyconfig			= lpddr4_writephyconfig,
		.writephyindepconfig		= lpddr4_writephyindepconfig,

		LPDDR4_CTL_REGS
		LPDDR4_PHY_INDEP_REGS
		LPDDR4_PHY_REGS

				CPS_REG_WRITE(lpddr4_addoffset(&(ctlregbase->DENALI_CTL_0), regoffset), regvalue);
				CPS_REG_WRITE(lpddr4_addoffset(&(ctlregbase->DENALI_PHY_0), regoffset), regvalue);
				CPS_REG_WRITE(lpddr4_addoffset(&(ctlregbase->DENALI_PI_0), regoffset), regvalue);
		}
	}
#endif