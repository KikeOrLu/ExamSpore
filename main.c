/**
 * @file    main.c
 * @brief   **Template Application entry point**
 *
 * The main file is the entry point of the application or any user code, please provide the 
 * proper description of this file according to your own implementation
 * This Demo app only blinks an LED connected to PortA Pin 5
 *
 * @note    Only the files inside folder app will be take them into account when the 
 *          doxygen runs by typing "make docs", index page is generated in
 *          Build/doxigen/html/index.html
 */

#include "app_bsp.h"

GPIO_InitTypeDef GPIO_InitStruct; /*creamos estructura gpios*/
TIM_HandleTypeDef TIM_Handler;    /*creamos estructura TIM*/

int main( void )
{
    HAL_Init();             /*Inicializamos la librería*/
    __GPIOC_CLK_ENABLE();   /*habilitamos reloj del puerto A*/
    __TIM6_CLK_ENABLE();    /*habilitamos reloj del Timer 6*/

    GPIO_InitStruct.Pin = GPIO_PIN_0;             /*pines a configurar*/
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   /*salida tipo push-pull*/
    GPIO_InitStruct.Pull = GPIO_NOPULL;           /*pin sin pull-up ni pull-down*/
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  /*pin a baja velocidad*/
    /*inicializamos pines con los parametros anteriores*/
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    /*configuramos timer 6 con preescaler de 1000 y overflow de 10000*/
    TIM_Handler.Instance = TIM6;
    TIM_Handler.Init.Prescaler = 1000;
    TIM_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_Handler.Init.Period = 10000;
    /*aplicamos configuracion al timer*/
    HAL_TIM_Base_Init( &TIM_Handler );

    /*Activamos interrupciones del timer TIM6*/
    HAL_NVIC_SetPriority( TIM6_DAC_LPTIM1_IRQn, 2, 0 );/*se fija la prioridad a nivel 2*/
    HAL_NVIC_EnableIRQ( TIM6_DAC_LPTIM1_IRQn );

    /*inciamos timer con interrupcion en overflow*/
    HAL_TIM_Base_Start_IT( &TIM_Handler );

    while (1)
    {

    }
}

/*funcion callback mandada llamar por la interrupcion del timer 6 
cuando este llega a su cuenta maxima de 10000*/
void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef *htim )
{
    HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_0 );
}