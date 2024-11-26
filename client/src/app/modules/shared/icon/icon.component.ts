import {
  ChangeDetectionStrategy,
  Component,
  EventEmitter,
  HostBinding,
  HostListener,
  Input,
  Output,
  ViewEncapsulation
} from '@angular/core';
import {SvgIcon, SvgIconMap} from "./svg-icons";
import { MatIconRegistry } from "@angular/material/icon";
import {DomSanitizer} from "@angular/platform-browser";
import {Type} from "./models/icon-config";

@Component({
  selector: 'app-icon',
  templateUrl: './icon.component.html',
  styleUrl: './icon.component.scss',
  encapsulation: ViewEncapsulation.None
})
export class IconComponent {
  @Input() type: Type;
  @Input() icon: SvgIcon | string;

  @Input()
  @HostBinding('style.width.px')
  @HostBinding('style.min-width.px')
  @HostBinding('style.height.px')
  @HostBinding('style.lineHeight.px')
  @HostBinding('style.fontSize.px')
  size = 24;

  // tslint:disable-next-line:no-output-native
  @Output() iconClick = new EventEmitter();

  @HostListener('click', ['$event'])
  onClick(event: Event) {
    this.iconClick.emit(event);
  }

  private numberOfChanges = 0;
  get showSvg(): boolean { 
    return !!this.getSvg();
  }
  constructor(private iconRegistry: MatIconRegistry,
              private domSanitizer: DomSanitizer) {}

  ngOnInit() {
    if (this.showSvg) {
        this.iconRegistry.addSvgIconLiteral(this.icon, this.domSanitizer.bypassSecurityTrustHtml(this.getSvg()));
    }
  }
  private getSvg(): string {
    return this.icon ? SvgIconMap[this.icon] : '';
  }
}
