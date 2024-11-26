export interface DictionaryNum<T> {
  [id: number]: T;
}

export abstract class Dictionary<T> implements DictionaryNum<T> {
  [id: string]: T;
}
