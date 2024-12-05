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
  RunBot
}

export interface Config {
  disabled: boolean,
  type: Type,
  text: string,
  tooltip?: string
  actionType: ActionType
}
