import {Dictionary} from "../utils/dictionary";


const svgTag = (svgContent: string, viewbox = '0 0 24 24') => `<svg viewBox="${viewbox}">${svgContent}</svg>`;
export enum SvgIcon {
  TrashOutline = 'trash-can-outline',
  EditOutline = 'edit-outline',
}

export const SvgIconMap: Dictionary<string> = {
  [SvgIcon.TrashOutline]: svgTag(`<path fill="currentColor" d="M9,3V4H4V6H5V19A2,2 0 0,0 7,21H17A2,2 0 0,0 19,19V6H20V4H15V3H9M7,6H17V19H7V6M9,8V17H11V8H9M13,8V17H15V8H13Z" />`),
  [SvgIcon.EditOutline]: svgTag(`<g id="Complete"><g id="edit"><g><path d="M20,16v4a2,2,0,0,1-2,2H4a2,2,0,0,1-2-2V6A2,2,0,0,1,4,4H8" fill="none" stroke="#000000" stroke-linecap="round" stroke-linejoin="round" stroke-width="2"/><polygon fill="none" points="12.5 15.8 22 6.2 17.8 2 8.3 11.5 8 16 12.5 15.8" stroke="#000000" stroke-linecap="round" stroke-linejoin="round" stroke-width="2"/></g></g></g>`),
}
