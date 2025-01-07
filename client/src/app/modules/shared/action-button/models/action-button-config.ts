export enum Type {
  Default,
  Info,
  Warn,
  Error
}

export enum ActionType {
  Create,
  Reload,
  Edit,
  Delete,
  Save,
  Close,
  Submit,
  SpecialAction1,
  SpecialAction2,
}

export interface Config {
  disabled: boolean,
  type: Type,
  text: string,
  tooltip?: string
  actionType: ActionType
}
