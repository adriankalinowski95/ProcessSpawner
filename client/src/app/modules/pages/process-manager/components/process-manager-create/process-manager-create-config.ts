import { ActionHandlerType, DynamicFormConfig, DynamicFormControlType, ErrorHandlerType, GuiValidatorType } from "app/modules/shared/dynamic-form/models/dynamic-form-config";
import { CustomInputComponent } from "app/modules/shared/form-components/custom-input/custom-input.component";

export const processManagerCreateConfig: DynamicFormConfig = {
    stepper: false,
    title: 'test',
    errorHandlerType: ErrorHandlerType.Outer,
    actionPanelConfig: {
      actionHandlerType: ActionHandlerType.Inner,
    },
    data: [
    {
        groupName: 'nameGroup',
        formControlsData: [
            {
                key: 'name',
                value: '',
                label: 'Name',
                guiValidators: [
                    { type: GuiValidatorType.MaxLength, value: 25 }, 
                    { type: GuiValidatorType.Required, value: true }
                ],
                customValidators: [],
                type: DynamicFormControlType.Input,
                component: CustomInputComponent,
            },
        ]
    },
    {
      groupName: 'networkGroup',
      formControlsData: [
        {
            key: 'address',
            value: '',
            label: 'Address',
            guiValidators: [
                { type: GuiValidatorType.MaxLength, value: 25 }, 
                { type: GuiValidatorType.Required, value: true }
            ],
            customValidators: [],
            type: DynamicFormControlType.Input,
            component: CustomInputComponent,
        },
        {
            key: 'port',
            value: '',
            label: 'Port',
            guiValidators: [
                { type: GuiValidatorType.MaxLength, value: 10 }, 
                { type: GuiValidatorType.Required, value: true }
            ],
            customValidators: [],
            type: DynamicFormControlType.Input,
            component: CustomInputComponent,
        }
      ]
    }
    ]
  }