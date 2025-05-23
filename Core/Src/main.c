/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "remote_control.h"
#include "motor.h"
#include "DWT.h"
#include "motorUsr.h"
#include "struct_typedef.h"
#include "bsp_can.h"
#include "dart.h"
#include "minipc_protocol.h"
#include "referee.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void taskInit(){//只需要执行一次的初始化函数，在系统上电时执行
  //DWT计时器初始化
  DWT_init();
  DWTRefreshTimeMs();//测试成功
  //遥控器初始化
  remote_control_init();//测试成功
  //裁判系统初始化

  //电机和电机pid初始化
  //PID
  fp32 motor_speed_pid_3508[3]={10,0.001,0};
  fp32 motor_pos_pid_3508[3] = {1,0,1};
  fp32 motor_speed_pid_2006[3]={10,0,0};
  fp32 motor_pos_pid_2006[3] = {1,0,1};
  fp32 motor_speed_pid_6020[3]={65,1.0f,0};
  fp32 motor_pos_pid_6020[3] = {70,0.8f,0};
  fp32 motor_pos_pixel_pid_6020[3]={5,0,0};
  motorInit(&D2006_motor1, D2006_MOTOR1_ID, 0x200, 0xFFF, 0x200);//推杆电机
  motorInit(&D2006_motor2, D2006_MOTOR2_ID, 0x200, 0xFFF, 0x200);//弹夹电机
  motorInit(&D3508_motor1, D3508_MOTOR1_ID, 0x200, 0xFFF, 0x200);//摩擦轮
  motorInit(&D3508_motor2, D3508_MOTOR2_ID, 0x200, 0xFFF, 0x200);//摩擦轮
  motorInit(&D6020_motor1, D6020_MOTOR1_ID, 0x2FE, 0x2FF, 0x204);//YAW电机
  pid_init(&D6020_motor1.motor_speed_pid, motor_speed_pid_6020, 24000, 24000);  //YAW电机速度PID
  pid_init(&D6020_motor1.motor_pos_pid, motor_pos_pid_6020, 400, 400);  //YAW电机位置PID
  pid_init(&D6020_motor1.motor_pos_pixel_pid,motor_pos_pixel_pid_6020,400,400);//yaw电机视觉调整用位置pid
  pid_init(&D2006_motor1.motor_speed_pid, motor_speed_pid_2006, 16000, 16000); //2006电机速度PID
  pid_init(&D2006_motor2.motor_speed_pid, motor_speed_pid_2006, 16000, 16000); //2006电机速度PID
  pid_init(&D2006_motor1.motor_pos_pid, motor_pos_pid_2006, 10000, 10000); //2006电机位置PID
  pid_init(&D2006_motor2.motor_pos_pid, motor_pos_pid_2006, 10000, 10000); //2006电机位置PID
  pid_init(&D3508_motor1.motor_speed_pid, motor_speed_pid_3508, 215, 16000); //3508电机速度PID
  pid_init(&D3508_motor2.motor_speed_pid, motor_speed_pid_3508, 215, 16000); //3508电机速度PID
  pid_init(&D3508_motor1.motor_pos_pid, motor_pos_pid_3508, 10000, 10000); //3508电机速位置PID
  pid_init(&D3508_motor2.motor_pos_pid, motor_pos_pid_3508, 10000, 10000); //3508电机速位置PID
  //can初始化
  can_filter_init();//对can1 can2的过滤器进行配置
  //飞镖创建、发射任务设置

  roket BYD4={.shootSpeed=3100,.yawPlace=970,.yawDelta=0};
  roket BB={.shootSpeed=3200,.yawPlace=860,.yawDelta=0};
  roket DD={.shootSpeed=3000,.yawPlace=870,.yawDelta=0};
	roket WC2={.shootSpeed=3100,.yawPlace=970,.yawDelta=0};

  roket SJ={.shootSpeed=3130,.yawPlace=970,.yawDelta=120,.shootSpeedBase=3775,.yawPlaceBase=3136,.yawDeltaBase=176};//+5
  roket PP={.shootSpeed=3100,.yawPlace=970,.yawDelta=120,.shootSpeedBase=3775,.yawPlaceBase=3099,.yawDeltaBase=163};//+5
  roket NC={.shootSpeed=3080,.yawPlace=970,.yawDelta=77,.shootSpeedBase=3775,.yawPlaceBase=3145,.yawDeltaBase=176};//+5
  roket NH={.shootSpeed=3070,.yawPlace=940,.yawDelta=65,.shootSpeedBase=3775,.yawPlaceBase=3091,.yawDeltaBase=157};//+10  

  roket HN={.shootSpeed=3140,.yawPlace=970,.yawDelta=37,.shootSpeedBase=3815,.yawPlaceBase=2700,.yawDeltaBase=185};//3140	
	
  roket GD={.shootSpeed=3140,.yawPlace=970,.yawDelta=37,.shootSpeedBase=3775,.yawPlaceBase=3150,.yawDeltaBase=181};//3140
  roket HRL={.shootSpeed=3175,.yawPlace=970,.yawDelta=73,.shootSpeedBase=3775,.yawPlaceBase=3149,.yawDeltaBase=181};//3175
  roket ZJ={.shootSpeed=3185,.yawPlace=970,.yawDelta=75,.shootSpeedBase=3790,.yawPlaceBase=3070,.yawDeltaBase=150};
  roket ZTX={.shootSpeed=3185,.yawPlace=970,.yawDelta=57,.shootSpeedBase=3775,.yawPlaceBase=2944,.yawDeltaBase=105};

  roket tet1={.shootSpeed=500,.yawPlace=500,.yawDelta=37,.shootSpeedBase=4000,.yawPlaceBase=2700,.yawDeltaBase=200};//偏左摩擦轮速度慢
  roket tet2={.shootSpeed=3175,.yawPlace=1500,.yawDelta=73,.shootSpeedBase=1000,.yawPlaceBase=700,.yawDeltaBase=-200};//偏右摩擦轮速度块
  roket tet3={.shootSpeed=3500,.yawPlace=2500,.yawDelta=75,.shootSpeedBase=4000,.yawPlaceBase=2700,.yawDeltaBase=200};//偏左摩擦轮速度很快
  roket tet4={.shootSpeed=3175,.yawPlace=300,.yawDelta=57,.shootSpeedBase=1000,.yawPlaceBase=700,.yawDeltaBase=-200};//偏右摩擦轮速度慢
  //shootTaskInit(&tet1,&tet2,&tet3,&tet4);//按照1、2、3、4发射顺序填入飞镖
	  shootTaskInit(&SJ,&PP,&NC,&NH);//按照1、2、3、4发射顺序填入飞镖
  //shootTaskInit(&GD,&HRL,&ZJ,&ZTX);//按照1、2、3、4发射顺序填入飞镖
  // shootTaskInit(&HN,&ZJ,&ZTX,&ZTX);
	//推杆电机和yaw电机初始化，方便限制电机动作
  while(!pushYawInit()){DWTRefreshTimeMs();//更新毫秒计时
    dartSysStateCheck();//各类限位状态查询
    checkControlMode();//遥控器选择控制模式
    minipc_upgrade(&minipc);//更新miniPC数据
    yawPlaceRefresh();//更新yaw电机数据
    pushPlaceRefreshSpeedy();//更新push电机数据
		calAndSendMotor();
		HAL_Delay(1);}
  //裁判系统接收初始化uart5
  HAL_UARTEx_ReceiveToIdle_IT(&huart5,rfDataBuf,sizeof(rfDataBuf));
  //其它初始化

}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
	taskInit();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    checkCangmenState();//弹舱门状态检测
    DWTRefreshTimeMs();//更新毫秒计时
    dartSysStateCheck();//各类限位状态查询
    checkControlMode();//遥控器选择控制模式
    minipc_upgrade(&minipc);//更新miniPC数据
    yawPlaceRefresh();//更新yaw电机数据
    pushPlaceRefreshSpeedy();//更新push电机数据
    switch (mode)
    {
      case manual:manualTask();break;
			case semiAuto:semiAutoTask();break;
      case fullAuto:fullAutoTask();break;
      default:break;
    }
    calAndSendMotor();
    HAL_Delay(1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
