/* generated HAL source file - do not edit */
#include "hal_data.h"
#if (5) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_button_interrupt) && !defined(SSP_SUPPRESS_ISR_ICU11)
SSP_VECTOR_DEFINE( icu_irq_isr, ICU, IRQ11);
#endif
#endif
static icu_instance_ctrl_t button_interrupt_ctrl;
static const external_irq_cfg_t button_interrupt_cfg =
{ .channel = 11,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = button_callback,
  .p_context = &button_interrupt,
  .p_extend = NULL,
  .irq_ipl = (5), };
/* Instance structure to use this module. */
const external_irq_instance_t button_interrupt =
{ .p_ctrl = &button_interrupt_ctrl, .p_cfg = &button_interrupt_cfg, .p_api = &g_external_irq_on_icu };
void g_hal_init(void)
{
    g_common_init ();
}
